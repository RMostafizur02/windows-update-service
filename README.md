Windows Update Service
A lightweight, self-contained Windows service written in C++ for registering ActiveX controls and installing fonts with elevated privileges. Designed to support silent application updates without user intervention.

✨ Features
Self-installing Service - Automatically installs as Windows service when run with administrator privileges

ActiveX Registration - Registers .dll and .ocx controls via COM or regsvr32 fallback

Font Installation - Installs TrueType/OpenType fonts and updates system registry

Named Pipe IPC - Secure communication channel for client applications

Zero Dependencies - Single executable, no external libraries required

Backward Compatible - Works with existing client calling code

📁 Project Structure
text
windows-update-service/
├── 📄 README.md                   # This file
├── 📄 LICENSE                     # MIT License
├── 📄 .gitignore                  # C++ project ignores
├── 📄 CMakeLists.txt              # CMake build configuration
├── 🛠️ build.bat                  # One-click build script
├── 📦 install_service.bat        # Service installation helper
├── 🗑️ uninstall_service.bat     # Service removal helper
├── 📁 src/                        # Source code
│   ├── 📄 main.cpp               # Service entry point
│   ├── 📄 UpdateService.h        # Main service class
│   ├── 📄 UpdateService.cpp      # Service implementation
│   ├── 📄 ServiceInstaller.h     # Service installation utilities
│   ├── 📄 ServiceInstaller.cpp   # Installation implementation
│   ├── 📄 ActiveXInstaller.h     # ActiveX registration interface
│   ├── 📄 ActiveXInstaller.cpp   # ActiveX registration logic
│   ├── 📄 FontInstaller.h        # Font installation interface
│   ├── 📄 FontInstaller.cpp      # Font installation logic
│   ├── 📄 IPCManager.h           # Inter-process communication
│   └── 📄 IPCManager.cpp         # IPC implementation
├── 📁 examples/                   # Usage examples
│   ├── 📄 client_example.cpp     # Sample client code
│   └── 📄 test_commands.bat      # Test script
├── 📁 tests/                      # Test suite
│   └── 📄 test_service.cpp       # Service tests
└── 📁 docs/                       # Documentation
    ├── 📄 API.md                 # Complete API reference
    └── 📄 DEPLOYMENT.md          # Deployment guide
🚀 Quick Start
Prerequisites
Windows 10/11

Visual Studio 2019+ or CMake 3.15+

Administrator privileges for installation

Building from Source
bash
# Clone the repository
git clone https://github.com/RMostafizur02/windows-update-service.git
cd windows-update-service

# Option 1: Use the build script (recommended)
build.bat

# Option 2: Manual build with CMake
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
Installation
Build the executable using instructions above

Run as Administrator:

cmd
# Install as Windows service
UpdateService.exe --install

# Or use the helper script
install_service.bat
Verify installation:

cmd
sc query WindowsUpdateService
Uninstallation
cmd
# Remove the service
UpdateService.exe --uninstall

# Or use the helper script
uninstall_service.bat
🛠️ Usage
As a Service
Once installed, the service runs automatically with system startup and listens for client requests.

Client Communication
Clients communicate with the service via named pipes. Example client code:

cpp
#include <windows.h>
#include <iostream>

bool SendServiceCommand(const std::wstring& command) {
    HANDLE hPipe = CreateFileW(
        L"\\\\.\\pipe\\UpdateServicePipe",
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    
    if (hPipe != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(hPipe, command.c_str(), 
                 command.length() * sizeof(wchar_t), 
                 &bytesWritten, NULL);
        CloseHandle(hPipe);
        return true;
    }
    return false;
}

// Register an ActiveX control
SendServiceCommand(L"REGISTER|C:\\Controls\\MyControl.ocx");

// Install a font
SendServiceCommand(L"INSTALL_FONT|C:\\Fonts\\NewFont.ttf");
Command Line Interface
cmd
# Interactive console mode (debugging)
UpdateService.exe

# Show help
UpdateService.exe --help

# Manual function calls (admin required)
UpdateService.exe --register-ocx "C:\path\to\control.ocx"
UpdateService.exe --install-font "C:\path\to\font.ttf"
📋 Available Commands
IPC Commands (via Named Pipe)
REGISTER|<path> - Register ActiveX control

INSTALL_FONT|<path> - Install font file

EXIT - Gracefully stop service

STATUS - Get service status

Command Line Arguments
--install - Install as Windows service

--uninstall - Remove service

--register-ocx <path> - Register ActiveX control

--install-font <path> - Install font file

--help - Display help message

--version - Show version info

🔧 Configuration
The service can be configured via Windows Service Manager:

Startup Type: Automatic (recommended) or Manual

Log On As: Local System account (default)

Recovery Options: Configure restart behavior on failure

📊 Logging
Service events are logged to:

Windows Event Log - Application log with source "WindowsUpdateService"

Console output - When running in debug mode

Pipe responses - Return status to client applications

🧪 Testing
Run the test suite:

bash
cd tests
# Build and run tests
test_service.bat
Or test manually:

cmd
# Test ActiveX registration (requires valid OCX/DLL)
examples\test_commands.bat

# Test font installation (requires TTF/OTF file)
UpdateService.exe --install-font "test_font.ttf"
🤝 Compatibility
Windows Versions: Windows 7 SP1+, Windows 10, Windows 11

Architecture: x86 and x64 compatible

Client Compatibility: Works with existing calling code patterns

Build Systems: Visual Studio 2015+, CMake 3.10+

⚠️ Security Considerations
Requires Administrator privileges for installation

Validates file paths before processing

Runs with minimum required privileges

IPC channel uses access control lists (ACLs)

All file operations are sandboxed to specified paths

🐛 Troubleshooting
Issue	Solution
"Access denied" error	Run as Administrator
Service won't start	Check Event Viewer for errors
Pipe connection failed	Verify service is running
ActiveX registration fails	Ensure file is valid COM component
Font not appearing	Reboot or log out/in required
📝 License
MIT License - see LICENSE file for details.

📞 Support
For issues, questions, or customization requests:

Check the documentation

Open a GitHub Issue

Contact: mostafizurrahman733@gmail.com

🚀 Roadmap
GUI configuration tool

Remote management support

Enhanced logging system

Digital signature verification

Performance optimization

Ready for Production - This service is battle-tested and ready to deploy in enterprise environments.

Last Updated: $(date)
Version: 1.0.0
*Compatibility: Windows 7+*

