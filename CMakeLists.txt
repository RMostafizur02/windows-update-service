cmake_minimum_required(VERSION 3.15)
project(UpdateService VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows specific settings
add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN)

# Executable
add_executable(UpdateService
    src/main.cpp
    src/UpdateService.cpp
    src/ServiceInstaller.cpp
    src/ActiveXInstaller.cpp
    src/FontInstaller.cpp
    src/IPCManager.cpp
)

# Windows libraries
target_link_libraries(UpdateService
    shell32.lib
    shlwapi.lib
    advapi32.lib
    user32.lib
)

# For service executable
set_target_properties(UpdateService PROPERTIES
    WIN32_EXECUTABLE TRUE
    LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

# Example client
add_executable(ServiceClient examples/client_example.cpp)
target_link_libraries(ServiceClient shell32.lib advapi32.lib)
