#include "sslserver.h"

#include "debug.h"
#include "service.h"

#include <QCoreApplication>
#include <QTimer>

#ifdef Q_OS_WIN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>
#include <tcpmib.h>

#include <softpub.h>
#include <wincrypt.h>
#include <wintrust.h>

#include <cstring>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Wintrust.lib")
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace {

void setFailureReason(QString *failureReason, const QString &reason)
{
    if (failureReason) {
        *failureReason = reason;
    }
}

bool findClientProcessId(int socketDescriptor, DWORD *processId, QString *failureReason)
{
    // Determine the process owning the accepted TCP connection.
    if (!processId) {
        setFailureReason(failureReason, QLatin1String("No process-id output was supplied."));
        return false;
    }

    const SOCKET socket = static_cast<SOCKET>(static_cast<quintptr>(socketDescriptor));
    sockaddr_storage peerAddress;
    sockaddr_storage localAddress;
    int peerAddressSize = sizeof(peerAddress);
    int localAddressSize = sizeof(localAddress);

    ZeroMemory(&peerAddress, sizeof(peerAddress));
    ZeroMemory(&localAddress, sizeof(localAddress));

    if (getpeername(socket, reinterpret_cast<sockaddr *>(&peerAddress), &peerAddressSize) == SOCKET_ERROR ||
        getsockname(socket, reinterpret_cast<sockaddr *>(&localAddress), &localAddressSize) == SOCKET_ERROR) {
        setFailureReason(failureReason, QString::fromLatin1("Could not inspect the local TCP connection (WSA error %1).").arg(WSAGetLastError()));
        return false;
    }

    if (peerAddress.ss_family != localAddress.ss_family) {
        setFailureReason(failureReason, QLatin1String("The TCP connection has incompatible address families."));
        return false;
    }

    const ULONG addressFamily = peerAddress.ss_family;
    DWORD tableSize = 0;
    DWORD result = GetExtendedTcpTable(0, &tableSize, FALSE, addressFamily, TCP_TABLE_OWNER_PID_ALL, 0);
    if (result != ERROR_INSUFFICIENT_BUFFER || tableSize == 0) {
        setFailureReason(failureReason, QString::fromLatin1("Could not size the TCP ownership table (error %1).").arg(result));
        return false;
    }

    std::vector<BYTE> tableBuffer(tableSize);
    result = GetExtendedTcpTable(&tableBuffer[0], &tableSize, FALSE, addressFamily, TCP_TABLE_OWNER_PID_ALL, 0);
    if (result != NO_ERROR) {
        setFailureReason(failureReason, QString::fromLatin1("Could not read the TCP ownership table (error %1).").arg(result));
        return false;
    }

    if (addressFamily == AF_INET) {
        // Match the accepted IPv4 connection against the TCP ownership table.
        const sockaddr_in *peer = reinterpret_cast<const sockaddr_in *>(&peerAddress);
        const sockaddr_in *local = reinterpret_cast<const sockaddr_in *>(&localAddress);
        const PMIB_TCPTABLE_OWNER_PID table = reinterpret_cast<PMIB_TCPTABLE_OWNER_PID>(&tableBuffer[0]);

        for (DWORD index = 0; index < table->dwNumEntries; ++index) {
            const MIB_TCPROW_OWNER_PID &row = table->table[index];
            if (row.dwState == MIB_TCP_STATE_ESTAB &&
                row.dwLocalAddr == peer->sin_addr.S_un.S_addr &&
                row.dwLocalPort == peer->sin_port &&
                row.dwRemoteAddr == local->sin_addr.S_un.S_addr &&
                row.dwRemotePort == local->sin_port) {
                *processId = row.dwOwningPid;
                return true;
            }
        }
    } else if (addressFamily == AF_INET6) {
        // Match the accepted IPv6 connection against the TCP ownership table.
        const sockaddr_in6 *peer = reinterpret_cast<const sockaddr_in6 *>(&peerAddress);
        const sockaddr_in6 *local = reinterpret_cast<const sockaddr_in6 *>(&localAddress);
        const PMIB_TCP6TABLE_OWNER_PID table = reinterpret_cast<PMIB_TCP6TABLE_OWNER_PID>(&tableBuffer[0]);

        for (DWORD index = 0; index < table->dwNumEntries; ++index) {
            const MIB_TCP6ROW_OWNER_PID &row = table->table[index];
            if (row.dwState == MIB_TCP_STATE_ESTAB &&
                std::memcmp(row.ucLocalAddr, &peer->sin6_addr, sizeof(row.ucLocalAddr)) == 0 &&
                row.dwLocalScopeId == peer->sin6_scope_id &&
                row.dwLocalPort == peer->sin6_port &&
                std::memcmp(row.ucRemoteAddr, &local->sin6_addr, sizeof(row.ucRemoteAddr)) == 0 &&
                row.dwRemoteScopeId == local->sin6_scope_id &&
                row.dwRemotePort == local->sin6_port) {
                *processId = row.dwOwningPid;
                return true;
            }
        }
    } else {
        setFailureReason(failureReason, QString::fromLatin1("Unsupported TCP address family %1.").arg(addressFamily));
        return false;
    }

    setFailureReason(failureReason, QLatin1String("The local client process could not be determined from the TCP connection."));
    return false;
}

bool processImagePath(DWORD processId, QString *imagePath, QString *failureReason)
{
    // Resolve the executable path of the process owning the client connection.
    if (!imagePath) {
        setFailureReason(failureReason, QLatin1String("No process-path output was supplied."));
        return false;
    }

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    if (!process) {
        setFailureReason(failureReason, QString::fromLatin1("Could not open client process %1 (error %2).").arg(processId).arg(GetLastError()));
        return false;
    }

    std::vector<wchar_t> pathBuffer(32768);
    DWORD pathLength = static_cast<DWORD>(pathBuffer.size());
    const BOOL result = QueryFullProcessImageNameW(process, 0, &pathBuffer[0], &pathLength);
    CloseHandle(process);

    if (!result) {
        setFailureReason(failureReason, QString::fromLatin1("Could not read the image path for client process %1 (error %2).").arg(processId).arg(GetLastError()));
        return false;
    }

    *imagePath = QString::fromWCharArray(&pathBuffer[0], static_cast<int>(pathLength));
    return true;
}

bool verifyAuthenticodeSignature(const QString &filePath, QString *failureReason)
{
    // Verify that the executable has a valid Authenticode signature.
    WINTRUST_FILE_INFO fileInfo;
    ZeroMemory(&fileInfo, sizeof(fileInfo));
    fileInfo.cbStruct = sizeof(fileInfo);
    fileInfo.pcwszFilePath = reinterpret_cast<LPCWSTR>(filePath.utf16());

    WINTRUST_DATA trustData;
    ZeroMemory(&trustData, sizeof(trustData));
    trustData.cbStruct = sizeof(trustData);
    trustData.dwUIChoice = WTD_UI_NONE;
    trustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    trustData.dwUnionChoice = WTD_CHOICE_FILE;
    trustData.pFile = &fileInfo;
    trustData.dwStateAction = WTD_STATEACTION_VERIFY;

    GUID policy = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    const LONG verificationResult = WinVerifyTrust(0, &policy, &trustData);

    trustData.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(0, &policy, &trustData);

    if (verificationResult != ERROR_SUCCESS) {
        setFailureReason(failureReason, QString::fromLatin1("Authenticode validation failed for %1 (error 0x%2).")
                         .arg(filePath, QString::number(static_cast<DWORD>(verificationResult), 16)));
        return false;
    }

    return true;
}

bool signerCertificateDer(const QString &filePath, QByteArray *encodedCertificate, QString *failureReason)
{
    // Extract the DER-encoded certificate used to sign the executable.
    if (!encodedCertificate) {
        setFailureReason(failureReason, QLatin1String("No certificate output was supplied."));
        return false;
    }

    HCERTSTORE certificateStore = 0;
    HCRYPTMSG cryptographicMessage = 0;
    PCCERT_CONTEXT signerCertificate = 0;
    bool success = false;

    do {
        DWORD encoding = 0;
        DWORD content = 0;
        DWORD format = 0;
        if (!CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                              reinterpret_cast<LPCWSTR>(filePath.utf16()),
                              CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                              CERT_QUERY_FORMAT_FLAG_BINARY,
                              0,
                              &encoding,
                              &content,
                              &format,
                              &certificateStore,
                              &cryptographicMessage,
                              0)) {
            setFailureReason(failureReason, QString::fromLatin1("Could not read the embedded signature from %1 (error %2).").arg(filePath).arg(GetLastError()));
            break;
        }

        DWORD signerInfoSize = 0;
        if (!CryptMsgGetParam(cryptographicMessage, CMSG_SIGNER_INFO_PARAM, 0, 0, &signerInfoSize) || signerInfoSize == 0) {
            setFailureReason(failureReason, QString::fromLatin1("Could not read signer information from %1 (error %2).").arg(filePath).arg(GetLastError()));
            break;
        }

        std::vector<BYTE> signerInfoBuffer(signerInfoSize);
        if (!CryptMsgGetParam(cryptographicMessage, CMSG_SIGNER_INFO_PARAM, 0, &signerInfoBuffer[0], &signerInfoSize)) {
            setFailureReason(failureReason, QString::fromLatin1("Could not retrieve signer information from %1 (error %2).").arg(filePath).arg(GetLastError()));
            break;
        }

        const PCMSG_SIGNER_INFO signerInfo = reinterpret_cast<PCMSG_SIGNER_INFO>(&signerInfoBuffer[0]);
        CERT_INFO certificateInfo;
        ZeroMemory(&certificateInfo, sizeof(certificateInfo));
        certificateInfo.Issuer = signerInfo->Issuer;
        certificateInfo.SerialNumber = signerInfo->SerialNumber;

        signerCertificate = CertFindCertificateInStore(certificateStore,
                                                       X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
                                                       0,
                                                       CERT_FIND_SUBJECT_CERT,
                                                       &certificateInfo,
                                                       0);
        if (!signerCertificate) {
            setFailureReason(failureReason, QString::fromLatin1("Could not find the signing certificate for %1 (error %2).").arg(filePath).arg(GetLastError()));
            break;
        }

        *encodedCertificate = QByteArray(reinterpret_cast<const char *>(signerCertificate->pbCertEncoded),
                                         static_cast<int>(signerCertificate->cbCertEncoded));
        success = true;
    } while (false);

    if (signerCertificate) {
        CertFreeCertificateContext(signerCertificate);
    }
    if (cryptographicMessage) {
        CryptMsgClose(cryptographicMessage);
    }
    if (certificateStore) {
        CertCloseStore(certificateStore, 0);
    }

    return success;
}

bool clientUsesSameCodeSigningCertificate(int socketDescriptor, QString *failureReason)
{
    // Identify the connecting process and verify both executables before
    // comparing their code-signing certificates.
    DWORD clientProcessId = 0;
    QString clientPath;
    QString servicePath = QCoreApplication::applicationFilePath();
    QByteArray clientCertificate;
    QByteArray serviceCertificate;

    if (!findClientProcessId(socketDescriptor, &clientProcessId, failureReason) ||
        !processImagePath(clientProcessId, &clientPath, failureReason) ||
        !verifyAuthenticodeSignature(servicePath, failureReason) ||
        !verifyAuthenticodeSignature(clientPath, failureReason) ||
        !signerCertificateDer(servicePath, &serviceCertificate, failureReason) ||
        !signerCertificateDer(clientPath, &clientCertificate, failureReason)) {
        return false;
    }

    if (serviceCertificate != clientCertificate)
    {
        // Only clients signed with the exact same certificate as the service
        // are allowed to connect.
        setFailureReason(failureReason, QString::fromLatin1("Client process %1 (%2) is not signed with the service signing certificate.")
                         .arg(clientProcessId)
                         .arg(clientPath));
        return false;
    }

    return true;
}

void logRejectedClient(const QString &reason)
{
    const QString message = QLatin1String("Rejected local client connection because its Authenticode signing certificate does not match the service: ") + reason;
    Debug::error(message);

    if (Service::instance()) {
        Service::instance()->logMessage(message, QtServiceBase::Error, 1001);
    }
}

} // namespace
#endif

extern bool g_bPortable;

SslServer::SslServer(quint16 port, QObject *parent)
    : QTcpServer(parent),
      connections (new SslConnections)
{
    //
    // Building a TCP Server which is listen at the port
    //
    listen(QHostAddress::LocalHost, port);

    Debug::log(QLatin1String("SSL server seems started succesfully"));
}

SslServer::~SslServer()
{
    //
    // Clear the connections list when programs ends
    //
    this->connections->clear();
}

void SslServer::incomingConnection(int socketDescriptor)
{
#ifdef Q_OS_WIN
#if !defined(DISABLE_CLIENT_SIGNATURE_CHECK) || defined(QT_NO_DEBUG)

    // Reject the connection before creating a client handler if the
    // connecting executable does not use the service signing certificate.
    QString signatureFailureReason;

    if (!clientUsesSameCodeSigningCertificate(socketDescriptor, &signatureFailureReason)) {
        logRejectedClient(signatureFailureReason);
        closesocket(static_cast<SOCKET>(static_cast<quintptr>(socketDescriptor)));
        return;
    }

#endif
#endif

    //
    // A new connection is incomming, create a new connection object and append it to the connection list
    //
    Debug::log(QLatin1String("New connection"));
    SslServerConnection *newIncommingConnection = new SslServerConnection (socketDescriptor, this);

    // Add the connection to the current connected list
    this->connections->add(newIncommingConnection);

    // When we receives the connectionIsClosed signal we exit the eventloop in the thread
    QObject::connect(newIncommingConnection, SIGNAL(connectionIsClosed()), this, SLOT(slotQuitThread()));
    // When the thread is finished cleanup
    QObject::connect(newIncommingConnection, SIGNAL(finished()), this, SLOT(slotThreadFinished()));

    // Start thread now
    newIncommingConnection->start(QThread::NormalPriority);
    newIncommingConnection->setId(this->connections->indexOf(newIncommingConnection));
    Debug::log(QLatin1String("Concurrent threads: ") + QString::number(this->connections->count()));
}

SslConnections *SslServer::connectionsList() const
{
    //
    // Return the connection list
    //
    return this->connections;
}

void SslServer::slotThreadFinished()
{
    //
    // Thread is done, mark the object to delete
    //

    // Remove the connection from the connection list
    SslServerConnection *con = qobject_cast<SslServerConnection*>(sender());
    Q_ASSERT(con);

    if (con) {
        // Remove item with thread id from the list
        this->connections->remove(con->threadId());
    }
    // mark object to delete
    sender()->deleteLater();

    if(connections->count() == 0)
    {
        // Close the serivce if we are in portable mode as the last client has disconnected
        if(g_bPortable)
        {
            //
            QTimer::singleShot(300, Qt::TimerType::CoarseTimer, this, SLOT(OnStopService()));
        }
    }
}

void SslServer::OnStopService()
{
    QCoreApplication::exit();
}

void SslServer::slotQuitThread()
{
    //
    // Stop the thread
    //
    SslServerConnection *con = qobject_cast<SslServerConnection*>(sender());
    Q_ASSERT(con);

    if (!con) {
        return;
    }
    // Object is valid
    con->quit();
    // Wait 5 seconds to end the thread, then terminate it
    if (!con->wait(5000)) {
        con->terminate();
        // Wait 2 sec
        con->wait(2000);
    }
}
