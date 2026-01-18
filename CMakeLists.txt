cmake_minimum_required(VERSION 3.15)
project(true-recall)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(true-recall
    src/main.cpp
    src/FocusTracker.cpp
    src/MonitorManager.cpp
    src/HotkeyManager.cpp
    src/TrayIcon.cpp
    src/Config.cpp
)

if(WIN32)
    target_link_libraries(true-recall PRIVATE user32 shell32)
endif()

# Set subsystem based on build type
if(MSVC)
    # Debug builds: Console window for output
    # Release builds: GUI mode (no console)
    set_target_properties(true-recall PROPERTIES
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
        LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()
