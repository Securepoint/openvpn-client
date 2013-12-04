#include "servicecontroller.h"
#include <tchar.h>

ServiceController::ServiceController(QWidget *parent) :
    QDialog(parent) {
    // Timer für den Dienst
    servTimer = new QTimer(this);
    connect(servTimer, SIGNAL(timeout()), this, SLOT(checkService()));
    serviceRunning = false;
}

void ServiceController::startService() {
    // Need WinAPI to controll the service
    if (!this->isServiceRunning()) {
        SC_HANDLE schSCManager;
        SC_HANDLE schService;

        SERVICE_STATUS ssStatus;
        DWORD dwOldCheckPoint;
        DWORD dwStartTickCount;
        DWORD dwWaitTime;

        // Paras for Start Service
        DWORD dwArgc = 2;
        // Pointer to arguments array
        // Field 0 = service name
        // Field 1 = config to load
        LPCTSTR *lpszArgv = new LPCTSTR[2];

        lpszArgv[0] = (LPTSTR) (LPCSTR) QString ("OpenVPNService").utf16();
        lpszArgv[1] = (LPTSTR) (LPCSTR) QString ("Simple").utf16();



        // Auf Parent zugreifen
        Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());
        prefDialog->setIcon(Connecting);
        prefDialog->showTrayMessage("Starting service ...", QSystemTrayIcon::Information);

        // Connect to the SC Manager
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

        if (NULL == schSCManager) {
            // Connetion faild
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Create handle to SC Manager failed!", QSystemTrayIcon::Critical);
            return;
        }

        // Try to open OpenVPN service
        schService = OpenService(schSCManager, (WCHAR*) QString ("OpenVPNService").utf16(), SERVICE_START | SERVICE_QUERY_STATUS);

        if (schService == NULL) {
            // Open service failed
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Open Service SC Manager failed!", QSystemTrayIcon::Critical);
            return;
        }

        if (!StartService(schService, dwArgc, lpszArgv)) {
            // Service cant be started
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Starting service failed!", QSystemTrayIcon::Critical);
            return;
        }

        if (!QueryServiceStatus(schService, &ssStatus)) {
            // Service cant be query
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Service query failed!", QSystemTrayIcon::Critical);
            return;
        }

        dwStartTickCount = GetTickCount();
        dwOldCheckPoint = ssStatus.dwCheckPoint;

        while (ssStatus.dwCurrentState == SERVICE_START_PENDING) {
            dwWaitTime = ssStatus.dwWaitHint / 10;

            if( dwWaitTime < 1000 )
                dwWaitTime = 1000;
            else if ( dwWaitTime > 5000 )
                dwWaitTime = 5000;

                Sleep( dwWaitTime );

                // Check the status again.
                if (!QueryServiceStatus(schService, &ssStatus))
                    break;

                if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
                {
                    // The service is making progress.
                    dwStartTickCount = GetTickCount();
                    dwOldCheckPoint = ssStatus.dwCheckPoint;
                }
                else
                {
                    if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
                    {
                        break;
                        }
                }
        } // end while

        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);

        if (ssStatus.dwCurrentState != SERVICE_RUNNING) {
            // Service didnt started
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Service didnt started!", QSystemTrayIcon::Critical);
            return;
        }

        // Dienst gestartet und connected
        prefDialog->setIcon(Connected);
        prefDialog->showTrayMessage("OpenVPN started successfully.", QSystemTrayIcon::Information);

        serviceRunning = true;

        // Timer starten
        servTimer->start(3000);
    }
} // end startService

void ServiceController::stopService() {
    if (this->isServiceRunning()) {
        SC_HANDLE schSCManager;
        SC_HANDLE schService;
        SERVICE_STATUS ssStatus;

        Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());
        prefDialog->showTrayMessage("Stopping service ...", QSystemTrayIcon::Information);

        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

        if (NULL == schSCManager) {
            // Connetion faild
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Create handle to SC Manager failed!", QSystemTrayIcon::Critical);
            return;
        }

        schService = OpenService(schSCManager,  (WCHAR*) QString ("OpenVPNService").utf16(), SERVICE_STOP);

        if (schService == NULL) {
            // Open service failed
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Open Service SC Manager failed!", QSystemTrayIcon::Critical);
            return;
        }

        if (!ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus) ) {
            // Stop service failed
            prefDialog->setIcon(Error);
            prefDialog->showTrayMessage("Stop Service SC Manager failed!", QSystemTrayIcon::Critical);
            return;
        }

        prefDialog->setIcon(Inaktiv);
        prefDialog->showTrayMessage("Service stopped successfully.", QSystemTrayIcon::Information);
        // TImer stoppen
        servTimer->stop();

        serviceRunning = false;
    }
}

void ServiceController::restartService() {
    this->stopService();
    QTimer::singleShot(1500, this, SLOT(shotStart()));
}

void ServiceController::checkService() {
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    SERVICE_STATUS ssStatus;

    Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

    if (NULL == schSCManager) {
        // not running
        prefDialog->setIcon(Inaktiv);
        prefDialog->setStatusMessage(QString("OpenVPN service is stopped."));
        servTimer->stop();
        serviceRunning = false;
        return;
    }

    schService = OpenService(schSCManager,  (WCHAR*) QString ("OpenVPNService").utf16(), SERVICE_QUERY_STATUS);

    if (schService == NULL) {
        // not running
        prefDialog->setIcon(Inaktiv);
        prefDialog->setStatusMessage(QString("OpenVPN service is stopped."));
        servTimer->stop();
        serviceRunning = false;
        return;
    }

    if (!QueryServiceStatus(schService, &ssStatus)) {
        // not running
        prefDialog->setIcon(Inaktiv);
        prefDialog->setStatusMessage(QString("OpenVPN service is stopped."));
        servTimer->stop();
        serviceRunning = false;
        return;
    }

    if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
        // running
        prefDialog->setIcon(Connected);
        prefDialog->setStatusMessage(QString("OpenVPN service is running."));
        serviceRunning = true;
        if (!servTimer->isActive()) {
            servTimer->start(3000);
        }
    } else {
        // not running
        prefDialog->setIcon(Inaktiv);
        prefDialog->setStatusMessage(QString("OpenVPN service is stopped."));
        serviceRunning = false;
        servTimer->stop();
    }
}

void ServiceController::shotStart() {
    this->startService();
}

bool ServiceController::isServiceRunning() {
    this->checkService();
    return this->serviceRunning;
}
