#include <windows.h>
#include <iostream>
#include <string>

class UpdateServiceClient {
public:
    bool SendCommand(const std::wstring& command) {
        HANDLE hPipe = CreateFileW(
            L"\\\\.\\pipe\\UpdateServicePipe",
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            return false;
        }

        DWORD bytesWritten;
        BOOL result = WriteFile(
            hPipe,
            command.c_str(),
            command.length() * sizeof(wchar_t),
            &bytesWritten,
            NULL
        );

        CloseHandle(hPipe);
        return result != FALSE;
    }

    bool RegisterActiveX(const std::wstring& path) {
        return SendCommand(L"REGISTER_ACTIVEX|" + path);
    }

    bool InstallFont(const std::wstring& path) {
        return SendCommand(L"INSTALL_FONT|" + path);
    }

    bool ExitService() {
        return SendCommand(L"EXIT_SERVICE");
    }
};

int wmain() {
    UpdateServiceClient client;
    
    // Example usage
    if (client.RegisterActiveX(L"C:\\Controls\\Example.ocx")) {
        std::wcout << L"ActiveX registration requested\n";
    }
    
    if (client.InstallFont(L"C:\\Fonts\\NewFont.ttf")) {
        std::wcout << L"Font installation requested\n";
    }
    
    return 0;
}
