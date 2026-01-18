cmake_minimum_required(VERSION 3.15)
project(wayback)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

add_executable(wayback
    src/main.cpp
    src/FocusTracker.cpp
    src/MonitorManager.cpp
    src/HotkeyManager.cpp
)

if(WIN32)
    target_link_libraries(wayback PRIVATE user32)
endif()

# Ensure console subsystem for MSVC
if(MSVC)
    set_target_properties(wayback PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()
