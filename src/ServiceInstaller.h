#pragma once
#include <windows.h>
#include <string>

class ServiceInstaller {
public:
    static bool InstallService();
    static bool UninstallService();
    static bool IsServiceInstalled();
    
private:
    static const wchar_t* SERVICE_NAME;
    static const wchar_t* SERVICE_DISPLAY_NAME;
    static const wchar_t* SERVICE_DESCRIPTION;
    
    static bool CreateService();
    static bool DeleteService();
    static std::wstring GetExecutablePath();
};
