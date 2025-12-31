#pragma once
#include <windows.h>
#include <string>
#include <thread>
#include <memory>

class UpdateService {
public:
    UpdateService();
    ~UpdateService();

    // Service entry points
    static VOID WINAPI ServiceMain(DWORD argc, LPWSTR* argv);
    static VOID WINAPI ServiceCtrlHandler(DWORD ctrlCode);

    // Core functionality
    static bool RegisterActiveX(const std::wstring& dllPath);
    static bool InstallFont(const std::wstring& fontPath);

    // Service control
    void Run();
    void Stop();
    void RunDebug(); // For console debugging

private:
    static UpdateService* instance;
    SERVICE_STATUS serviceStatus;
    SERVICE_STATUS_HANDLE serviceStatusHandle;
    std::unique_ptr<std::thread> workerThread;
    bool isRunning;

    void ReportStatus(DWORD currentState, DWORD exitCode = 0, DWORD waitHint = 0);
    void ServiceWorker();
    void ProcessIPCRequests();
};
