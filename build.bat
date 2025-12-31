@echo off
echo Building Windows Update Service...

if not exist "build" mkdir build
cd build

echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

echo Building...
cmake --build . --config Release

echo.
echo Build complete!
echo Executable: build\Release\UpdateService.exe
pause
