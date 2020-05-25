
#include "crypt.h"
#include <Windows.h>
#include <wincrypt.h>
#include "dpapi.h"


QByteArray DPAPI::encodePlaintextUtf8(const QByteArray &inputUtf8)
{
    DATA_BLOB decryptedData;
    DATA_BLOB encryptedData;

    decryptedData.pbData = (BYTE*)inputUtf8.data();
    decryptedData.cbData = inputUtf8.length();

    if(!CryptProtectData(&decryptedData, L"SECUREPOINT_SSLVPN_VALUES", NULL, NULL, NULL, 0,&encryptedData)) {
        //MessageBoxA(NULL, "DPAPI encoding failed.", "Error", 0);
        qDebug() << "DPAPI encoding failed.";
        //
        return QByteArray();
    }

    // Return it as hex
    return (QByteArray::fromRawData((char*)encryptedData.pbData, encryptedData.cbData)).toHex();
}

QByteArray DPAPI::decodeToPlaintext(const QByteArray &inputHex)
{
    DATA_BLOB encryptedData;
    DATA_BLOB decryptedData;

    // Useless, we always use the same description
    LPWSTR descriptionString;

    // All crypted text is hex
    QByteArray bytesToDecrypt (QByteArray::fromHex(inputHex));

    encryptedData.pbData = (BYTE*)bytesToDecrypt.data();
    encryptedData.cbData = bytesToDecrypt.length();

    if(!CryptUnprotectData(&encryptedData, &descriptionString, NULL, NULL, NULL, 0, &decryptedData)) {
        //MessageBoxA(NULL, "DPAPI decoding failed.", "Error", 0);
        qDebug() << "DPAPI decoding failed.";
        //
        return QByteArray();
    }

    return QByteArray::fromRawData((char*)decryptedData.pbData, decryptedData.cbData);
}

QString Crypt::secretKey = "aes128-cbc-pkcs7-sn93-sh21-jks-1";
bool Crypt::exceptionEnabled = false;
bool Crypt::useDPAPI = false;

Crypt::Crypt()
    /* : secretKey("aes128-cbc-pkcs7-sn93-sh21-jks-12")*/
{
}

Crypt::~Crypt() {

}

#include <assert.h>

void Log(const char* szFormat, ...)
{
    char szBuff[1024];
    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    OutputDebugStringA(szBuff);
    OutputDebugStringA("\n");
}

void Crypt::enableDPAPI()
{
    useDPAPI = true;
}

bool Crypt::isDPAPIEnabled()
{
    return useDPAPI;
}

void Crypt::setSecretKey(const QString &key)
{
    //
    // Set the secret key
    //

    Q_UNUSED(key)
        secretKey = key;
}

typedef struct {
    BLOBHEADER	header;
    DWORD		key_length;
    BYTE		key_bytes[16];
}AesBlob128;

QStringList magicsKey()
{
    QStringList lst = QString("aes128-cbc-pkcs7-sn93-sh21-jks-12").split(QLatin1String("-"));
    lst = lst.replaceInStrings(QLatin1String("c"), QLatin1String("m"));
    lst.removeAt(0);
    lst.removeAt(5);
    return lst;
}

QString Crypt::encodePlaintext (const QString &plain)
{
    // Bridge the encode method to make sure we are
    // passing a utf8 byte array to the encoding method
    if (useDPAPI) {
        return DPAPI::encodePlaintextUtf8(plain.toUtf8());
    }

    // Default is no dpapi support
    return Crypt::encodePlaintextUtf8 (plain.toUtf8());
}

QString Crypt::encodePlaintextUtf8 (const QByteArray &plain)
{
    QByteArray encrypted;

    // Create the crypto provider context.
    HCRYPTPROV hProvider = NULL;
    if (!CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {

        if (GetLastError() == NTE_BAD_KEYSET)
        {
            if (!CryptAcquireContext(&hProvider, 
                NULL, 
                NULL, 
                PROV_RSA_AES, 
                CRYPT_NEWKEYSET))
            {
                char keks[MAX_PATH] = {0};
                sprintf(keks, "Last error %d", GetLastError());
                MessageBoxA(NULL, "Unable to create crypto provider context.", "Error", 0);
                throw std::runtime_error("Unable to create crypto provider context.");
            }
        }
    }

    int kAesBytes128 = 16;

    // Construct the blob necessary for the key generation.
    AesBlob128 aes_blob;
    aes_blob.header.bType = PLAINTEXTKEYBLOB;
    aes_blob.header.bVersion = CUR_BLOB_VERSION;
    aes_blob.header.reserved = 0;
    aes_blob.header.aiKeyAlg = CALG_AES_128;
    aes_blob.key_length = kAesBytes128;
    memcpy(aes_blob.key_bytes, secretKey.toLatin1().data(), kAesBytes128);

    if(secretKey.length() < kAesBytes128) {
        for(int i = secretKey.length(); i < kAesBytes128; ++i)
        {
            aes_blob.key_bytes[i] = 0;
        }
    }

    // Create the crypto key struct that Windows needs.
    HCRYPTKEY hKey = NULL;
    if (!CryptImportKey(hProvider, reinterpret_cast<BYTE*>(&aes_blob), sizeof(AesBlob128), NULL, 0, &hKey)) {
        if(exceptionEnabled) {
            throw std::runtime_error("Unable to create crypto key.");
        } else
             MessageBoxA(NULL, "Unable to create crypto key.", "Error", 0);
    }

    DWORD CryptMode = CRYPT_MODE_CBC;;
    CryptSetKeyParam(hKey, KP_MODE, (LPBYTE)&CryptMode, 0);

    CryptSetKeyParam(hKey, KP_IV, (const BYTE*)QVariant(magicsKey().join("<")).toByteArray().data(), 0);

    // The CryptEncrypt method uses the *same* buffer for both the input and
    // output (!), so we copy the data to be encrypted into the output array.
    // Also, for some reason, the AES-128 block cipher on Windows requires twice
    // the block size in the output buffer. So we resize it to that length and
    // then chop off the excess after we are done.
    encrypted.clear();
    encrypted.append(plain);


    // This acts as both the length of bytes to be encoded (on input) and the
    // number of bytes used in the resulting encrypted data (on output).
    DWORD length = encrypted.length();
    DWORD dwSize;

    // Set variable to length of data in buffer.
    dwSize = length;

    CryptEncrypt(hKey, NULL, true, 0, NULL, &dwSize, dwSize);

    encrypted.resize(dwSize);

    if (!CryptEncrypt(hKey, NULL, true, 0, reinterpret_cast<BYTE*>(encrypted.data()), &length, dwSize)) {
        if(exceptionEnabled)
            throw std::runtime_error("Encryption failed");
        else
             MessageBoxA(NULL, "Encryption failed.", "Error", 0);
    }

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProvider, 0);
    return encrypted.toHex();
}


QString Crypt::encodePlaintextAscii (const QByteArray &plain)
{
    QByteArray encrypted;

    // Create the crypto provider context.
    HCRYPTPROV hProvider = NULL;
    if (!CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {

        if (GetLastError() == NTE_BAD_KEYSET)
        {
            if (!CryptAcquireContext(&hProvider,
                NULL,
                NULL,
                PROV_RSA_AES,
                CRYPT_NEWKEYSET))
            {
                char keks[MAX_PATH] = {0};
                sprintf(keks, "Last error %d", GetLastError());
                MessageBoxA(NULL, "Unable to create crypto provider context.", "Error", 0);
                throw std::runtime_error("Unable to create crypto provider context.");
            }
        }
    }

    int kAesBytes128 = 16;

    // Construct the blob necessary for the key generation.
    AesBlob128 aes_blob;
    aes_blob.header.bType = PLAINTEXTKEYBLOB;
    aes_blob.header.bVersion = CUR_BLOB_VERSION;
    aes_blob.header.reserved = 0;
    aes_blob.header.aiKeyAlg = CALG_AES_128;
    aes_blob.key_length = kAesBytes128;
    memcpy(aes_blob.key_bytes, secretKey.toLatin1().data(), kAesBytes128);

    if(secretKey.length() < kAesBytes128) {
        for(int i = secretKey.length(); i < kAesBytes128; ++i)
        {
            aes_blob.key_bytes[i] = 0;
        }
    }

    // Create the crypto key struct that Windows needs.
    HCRYPTKEY hKey = NULL;
    if (!CryptImportKey(hProvider, reinterpret_cast<BYTE*>(&aes_blob), sizeof(AesBlob128), NULL, 0, &hKey)) {
        if(exceptionEnabled) {
            throw std::runtime_error("Unable to create crypto key.");
        } else
             MessageBoxA(NULL, "Unable to create crypto key.", "Error", 0);
    }

    DWORD CryptMode = CRYPT_MODE_CBC;;
    CryptSetKeyParam(hKey, KP_MODE, (LPBYTE)&CryptMode, 0);

    CryptSetKeyParam(hKey, KP_IV, (const BYTE*)QVariant(magicsKey().join("<")).toByteArray().data(), 0);

    // The CryptEncrypt method uses the *same* buffer for both the input and
    // output (!), so we copy the data to be encrypted into the output array.
    // Also, for some reason, the AES-128 block cipher on Windows requires twice
    // the block size in the output buffer. So we resize it to that length and
    // then chop off the excess after we are done.
    encrypted.clear();
    encrypted.append(plain);


    // This acts as both the length of bytes to be encoded (on input) and the
    // number of bytes used in the resulting encrypted data (on output).
    DWORD length = encrypted.length();
    DWORD dwSize;

    // Set variable to length of data in buffer.
    dwSize = length;

    CryptEncrypt(hKey, NULL, true, 0, NULL, &dwSize, dwSize);

    encrypted.resize(dwSize);

    if (!CryptEncrypt(hKey, NULL, true, 0, reinterpret_cast<BYTE*>(encrypted.data()), &length, dwSize)) {
        if(exceptionEnabled)
            throw std::runtime_error("Encryption failed");
        else
             MessageBoxA(NULL, "Encryption failed.", "Error", 0);
    }

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProvider, 0);
    return encrypted.toHex();
}
QByteArray Crypt::decodeToPlaintext (const QString &crypt) {
    //
    if (useDPAPI) {
        return DPAPI::decodeToPlaintext(crypt.toLocal8Bit());
    }

    // Default is no dpapi
    return Crypt::decodeToPlaintextWithKey(crypt);
}

QByteArray Crypt::decodeToPlaintextWithKey (const QString &crypt)
{
    QByteArray encrypted;

    // Create the crypto provider context.
    HCRYPTPROV hProvider = NULL;
    if (!CryptAcquireContext(&hProvider, NULL, NULL, PROV_RSA_AES, NULL)) {
        if (GetLastError() == NTE_BAD_KEYSET)
        {
            if (!CryptAcquireContext(&hProvider, 
                NULL, 
                NULL, 
                PROV_RSA_AES, 
                CRYPT_NEWKEYSET))
            {
                char keks[MAX_PATH] = {0};
                sprintf(keks, "Last error %d", GetLastError());
                MessageBoxA(NULL, "Unable to create crypto provider context.", "Error", 0);
                throw std::runtime_error("Unable to create crypto provider context.");
            }
        }
    }

    int kAesBytes128=16;

    // Construct the blob necessary for the key generation.
    AesBlob128 aes_blob;
    aes_blob.header.bType = PLAINTEXTKEYBLOB;
    aes_blob.header.bVersion = CUR_BLOB_VERSION;
    aes_blob.header.reserved = 0;
    aes_blob.header.aiKeyAlg = CALG_AES_128;
    aes_blob.key_length = kAesBytes128;
    memcpy(aes_blob.key_bytes, secretKey.toLatin1().data(), kAesBytes128);

    if(secretKey.length() < kAesBytes128) {
        for(int i = secretKey.length(); i < kAesBytes128; ++i)
        {
            aes_blob.key_bytes[i] = 0;
        }
    }
    // Create the crypto key struct that Windows needs.
    HCRYPTKEY hKey = NULL;
    if (!CryptImportKey(hProvider, reinterpret_cast<BYTE*>(&aes_blob), sizeof(AesBlob128), NULL, 0, &hKey)) {
        if(exceptionEnabled)
            throw std::runtime_error("Unable to create crypto key.");
        else
            MessageBoxA(NULL, "Unable to create crypto key.", "Error", 0);
    }

    DWORD CryptMode = CRYPT_MODE_CBC;;
    CryptSetKeyParam(hKey, KP_MODE, (LPBYTE)&CryptMode, 0);

    CryptSetKeyParam(hKey, KP_IV, (const BYTE*)QVariant(magicsKey().join("<")).toByteArray().data(), 0);

    //DWORD PadMode = PKCS5_PADDING;
    //CryptSetKeyParam(hKey, KP_PADDING, (LPBYTE)&PadMode, 0);

    // The CryptEncrypt method uses the *same* buffer for both the input and
    // output (!), so we copy the data to be encrypted into the output array.
    // Also, for some reason, the AES-128 block cipher on Windows requires twice
    // the block size in the output buffer. So we resize it to that length and
    // then chop off the excess after we are done.
    encrypted.clear();
    encrypted.append(QByteArray::fromHex(crypt.toLatin1()));

    DWORD length = kAesBytes128;
    DWORD len = encrypted.length();
    length = len;
    if (!CryptDecrypt(hKey, NULL, true, 0, reinterpret_cast<BYTE*>(encrypted.data()), &len)) {
        if(exceptionEnabled)
            throw std::runtime_error("Encryption failed");
        else {
            MessageBoxA(NULL, "Encryption failed.", "Error", 0);
        }
    }

    // See comment above.
    encrypted.chop(length - len);

    CryptDestroyKey(hKey);
    CryptReleaseContext(hProvider, 0);

    return encrypted;
}

