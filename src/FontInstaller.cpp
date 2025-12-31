#include "FontInstaller.h"
#include <windows.h>
#include <shlwapi.h>
#include <vector>

bool FontInstaller::Install(const std::wstring& fontPath) {
    // 1. Add font resource
    int fontCount = AddFontResourceExW(fontPath.c_str(), FR_PRIVATE, NULL);
    if (fontCount == 0) {
        return false;
    }

    // 2. Update registry
    HKEY hKey;
    std::wstring fontName = ExtractFontName(fontPath);
    std::wstring regPath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
    
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, regPath.c_str(), 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        std::wstring value = fontName + L" (TrueType)";
        RegSetValueExW(hKey, value.c_str(), 0, REG_SZ,
                      (const BYTE*)fontPath.c_str(),
                      (fontPath.length() + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
    }

    // Notify system of font change
    SendMessageTimeoutW(HWND_BROADCAST, WM_FONTCHANGE, 0, 0,
                       SMTO_NORMAL, 1000, NULL);
    
    return true;
}

std::wstring FontInstaller::ExtractFontName(const std::wstring& fontPath) {
    // Extract font name from path (simplified)
    size_t pos = fontPath.find_last_of(L"\\/");
    size_t dot = fontPath.find_last_of(L".");
    if (pos != std::wstring::npos && dot != std::wstring::npos) {
        return fontPath.substr(pos + 1, dot - pos - 1);
    }
    return L"UnknownFont";
}
