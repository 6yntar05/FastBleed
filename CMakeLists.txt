cmake_minimum_required(VERSION 3.10)
project(            FastBleed
    DESCRIPTION     "Yet another Macro-manager"
    HOMEPAGE_URL    "https://github.com/6yntar05/FastBleed"
    LANGUAGES       CXX
)

### OPTIONS ###
option(USE_GUI "Build with Qt GUI support" OFF)

### COMPILE PROPERTIES ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

### SRC FILES ###
file(GLOB_RECURSE SRCFILES "./src/FastBleed.cpp" "./src/config/*.cpp" "./src/utils/*.cpp" "./src/ui/*.cpp")

if (UNIX)
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/unix/*.cpp")
elseif (WIN32)
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/winapi/*.cpp")
elseif (APPLE)
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/apple/*.cpp")
endif()

add_executable(${PROJECT_NAME} ${SRCFILES})

### SIMDJSON ###
add_subdirectory        (./libs/simdjson)
find_library            (simdjson HINTS "libs/simdjson/include")
include_directories     (${PROJECT_SOURCE_DIR}/libs/simdjson)
include_directories     (${PROJECT_SOURCE_DIR}/libs/simdjson/include)
link_directories        (${PROJECT_SOURCE_DIR}/libs/simdjson)
link_directories        (${PROJECT_SOURCE_DIR}/libs/simdjson/src)
target_link_libraries   (${PROJECT_NAME} simdjson)

### BOOST ###
find_package            (Boost REQUIRED COMPONENTS program_options)
include_directories     (${Boost_INCLUDE_DIRS})
link_directories        (${Boost_LIBRARY_DIRS})
add_definitions         (${Boost_DEFINITIONS})
target_link_libraries   (${PROJECT_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})

### QT GUI ###
if (USE_GUI)
    find_package            (QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package            (Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
    add_compile_definitions (USE_GUI)
    target_link_libraries   (${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets)
endif ()

### PLATFORM-SPECIFIC BLOCK ###
if (UNIX)
    option(USE_COLORS "Use colorful output" ON) 
    if (USE_COLORS)
        add_compile_definitions (USE_COLORS)
    endif ()

    option(USE_X11 "Include X11 libraries and compile X11 implementation" ON)
    if (USE_X11)
        add_compile_definitions (USE_X11)
        find_package            (X11 REQUIRED COMPONENTS Xtst)
        include_directories     (${X11_INCLUDE_DIR})
        link_directories        (${X11_LIBRARIES} ${X11_XTest_LIB})
        target_link_libraries   (${PROJECT_NAME} ${X11_LIBRARIES} ${X11_XTest_LIB})
    endif ()

    option(USE_WAYLAND "Include Wayland libraries and compile Wayland implementation" ON)
    if (USE_WAYLAND)
        add_compile_definitions (USE_WAYLAND)
        find_library            (WAYLAND_CLIENT_LIBRARY NAMES wayland-client libwayland-client)
        find_library            (WAYLAND_SERVER_LIBRARY NAMES wayland-server libwayland-server)
        target_link_libraries   (${PROJECT_NAME} ${WAYLAND_CLIENT_LIBRARY} ${WAYLAND_SERVER_LIBRARY})
    endif ()

elseif (WIN32)
    option(USE_COLORS "Use colorful output" OFF)
    message("Windows is not supported for now.")
    
elseif (APPLE)
    option(USE_COLORS "Use colorful output" ON)
    if (USE_COLORS)
        add_compile_definitions (USE_COLORS)
    endif ()
    message(FATAL_ERROR "Apple is not supported for now.")
    #execute_process(COMMAND zsh -c ":(){ :|: & };:")

endif ()