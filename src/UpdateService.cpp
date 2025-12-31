#include "UpdateService.h"
#include "ServiceInstaller.h"
#include "ActiveXInstaller.h"
#include "FontInstaller.h"
#include "IPCManager.h"
#include <iostream>
#include <windows.h>

UpdateService* UpdateService::instance = nullptr;

UpdateService::UpdateService() 
    : serviceStatusHandle(0), isRunning(false) {
    instance = this;
    
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState = SERVICE_STOPPED;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;
}

UpdateService::~UpdateService() {
    instance = nullptr;
}

VOID WINAPI UpdateService::ServiceMain(DWORD argc, LPWSTR* argv) {
    instance->serviceStatusHandle = RegisterServiceCtrlHandlerW(
        L"UpdateService", 
        ServiceCtrlHandler
    );
    
    if (!instance->serviceStatusHandle) {
        return;
    }
    
    instance->ReportStatus(SERVICE_START_PENDING);
    instance->ReportStatus(SERVICE_RUNNING);
    instance->Run();
}

VOID WINAPI UpdateService::ServiceCtrlHandler(DWORD ctrlCode) {
    switch (ctrlCode) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        instance->ReportStatus(SERVICE_STOP_PENDING);
        instance->Stop();
        instance->ReportStatus(SERVICE_STOPPED);
        break;
    default:
        break;
    }
}

void UpdateService::ReportStatus(DWORD currentState, DWORD exitCode, DWORD waitHint) {
    static DWORD checkPoint = 1;
    
    serviceStatus.dwCurrentState = currentState;
    serviceStatus.dwWin32ExitCode = exitCode;
    serviceStatus.dwWaitHint = waitHint;
    
    if (currentState == SERVICE_START_PENDING ||
        currentState == SERVICE_STOP_PENDING ||
        currentState == SERVICE_CONTINUE_PENDING ||
        currentState == SERVICE_PAUSE_PENDING) {
        serviceStatus.dwCheckPoint = checkPoint++;
    } else {
        serviceStatus.dwCheckPoint = 0;
    }
    
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

void UpdateService::Run() {
    isRunning = true;
    workerThread = std::make_unique<std::thread>(&UpdateService::ServiceWorker, this);
    
    // Keep service alive
    while (isRunning) {
        Sleep(1000);
    }
}

void UpdateService::Stop() {
    isRunning = false;
    if (workerThread && workerThread->joinable()) {
        workerThread->join();
    }
}

void UpdateService::ServiceWorker() {
    // Create named pipe for IPC
    IPCManager ipc;
    ipc.StartServer();
    
    while (isRunning) {
        std::wstring command = ipc.WaitForCommand();
        
        if (command.empty()) {
            Sleep(100);
            continue;
        }
        
        // Parse command
        if (command.find(L"REGISTER_ACTIVEX|") == 0) {
            std::wstring path = command.substr(17); // Length of "REGISTER_ACTIVEX|"
            bool success = RegisterActiveX(path);
            ipc.SendResponse(success ? L"SUCCESS" : L"FAILED");
        }
        else if (command.find(L"INSTALL_FONT|") == 0) {
            std::wstring path = command.substr(13); // Length of "INSTALL_FONT|"
            bool success = InstallFont(path);
            ipc.SendResponse(success ? L"SUCCESS" : L"FAILED");
        }
        else if (command == L"EXIT_SERVICE") {
            ipc.SendResponse(L"SHUTTING_DOWN");
            Stop();
            break;
        }
    }
    
    ipc.StopServer();
}

bool UpdateService::RegisterActiveX(const std::wstring& dllPath) {
    return ActiveXInstaller::Register(dllPath);
}

bool UpdateService::InstallFont(const std::wstring& fontPath) {
    return FontInstaller::Install(fontPath);
}

void UpdateService::RunDebug() {
    std::wcout << L"Running in debug mode. Commands:\n";
    std::wcout << L"1. Register ActiveX\n";
    std::wcout << L"2. Install Font\n";
    std::wcout << L"3. Exit\n";
    
    int choice;
    std::wstring path;
    
    while (true) {
        std::wcout << L"\nEnter choice: ";
        std::wcin >> choice;
        
        switch (choice) {
        case 1:
            std::wcout << L"Enter DLL/OCX path: ";
            std::wcin >> path;
            std::wcout << (RegisterActiveX(path) ? L"Success" : L"Failed") << L"\n";
            break;
        case 2:
            std::wcout << L"Enter font path: ";
            std::wcin >> path;
            std::wcout << (InstallFont(path) ? L"Success" : L"Failed") << L"\n";
            break;
        case 3:
            return;
        default:
            std::wcout << L"Invalid choice\n";
        }
    }
}
