#include "ActiveXInstaller.h"
#include <windows.h>
#include <shellapi.h>

bool ActiveXInstaller::Register(const std::wstring& dllPath) {
    HMODULE hModule = LoadLibraryExW(dllPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!hModule) {
        return false;
    }

    FARPROC pDllRegisterServer = GetProcAddress(hModule, "DllRegisterServer");
    if (pDllRegisterServer) {
        HRESULT hr = ((HRESULT(WINAPI*)())pDllRegisterServer)();
        FreeLibrary(hModule);
        return SUCCEEDED(hr);
    }

    FreeLibrary(hModule);
    
    // Fallback to regsvr32
    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = L"regsvr32.exe";
    sei.lpParameters = (L"/s \"" + dllPath + L"\"").c_str();
    sei.nShow = SW_HIDE;
    
    return ShellExecuteExW(&sei) != FALSE;
}
