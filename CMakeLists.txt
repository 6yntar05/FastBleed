cmake_minimum_required(VERSION 3.10)
project(            FastBleed
    DESCRIPTION     "Yet another Macro-manager"
    HOMEPAGE_URL    "https://github.com/6yntar05/FastBleed"
    LANGUAGES       CXX
)

### OPTIONS ###
option(USE_GUI "Build with Qt GUI support" OFF)
option(USE_BOOST "Use BOOST:program_options" ON)

### COMPILE PROPERTIES ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -fdiagnostics-color=always -O3)

    if ("${CMAKE_BUILD_TYPE}" STRGREATER_EQUAL "DEBUG")
        add_compile_options(-O0 -g -fsanitize=address)
        add_link_options(-fsanitize=address)
    endif ()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -fcolor-diagnostics -O3)

    if ("${CMAKE_BUILD_TYPE}" STRGREATER_EQUAL "DEBUG")
        add_compile_options(-O0 -g -fsanitize=address)
        add_link_options(-fsanitize=address)
    endif ()

endif ()

### SRC FILES ###
file(GLOB SRCFILES "./src/*.cpp")                                               # In main dir
file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/utils/*.cpp" "./src/ui/*.cpp")    # Other dirs
if (UNIX)                                                                       # Compile-time platform selection
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/unix/*.cpp")
elseif (WIN32)
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/winapi/*.cpp")
elseif (APPLE)
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/apple/*.cpp")
endif()

include_directories(AFTER SYSTEM ./include)
add_executable(${PROJECT_NAME} ${SRCFILES})
target_include_directories(${PROJECT_NAME} PRIVATE ./include)

### SUBMODULES ###
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message (">> Update submodules")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

### SIMDJSON ###
add_subdirectory        (./libs/simdjson)
find_library            (simdjson HINTS "libs/simdjson/include")
include_directories     (${PROJECT_SOURCE_DIR}/libs/simdjson)
include_directories     (${PROJECT_SOURCE_DIR}/libs/simdjson/include)
link_directories        (${PROJECT_SOURCE_DIR}/libs/simdjson)
link_directories        (${PROJECT_SOURCE_DIR}/libs/simdjson/src)
target_link_libraries   (${PROJECT_NAME} simdjson)

### BOOST ###
message (">> Finding Boost")
find_package (Boost QUIET COMPONENTS program_options)
if (USE_BOOST)
    if (Boost_FOUND)
        add_compile_definitions (USE_BOOST)
        include_directories     (${Boost_INCLUDE_DIRS})
        link_directories        (${Boost_LIBRARY_DIRS})
        add_definitions         (${Boost_DEFINITIONS})
        target_link_libraries   (${PROJECT_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})
    else ()
        message (WARNING "BOOST Not found! Skipping")
    endif ()
endif ()

### QT GUI ###
message (">> Finding Qt")
find_package(QT NAMES Qt6 Qt5 QUIET COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Widgets)
if (USE_GUI)
    if (QT_FOUND)
        add_compile_definitions (USE_GUI)
        target_link_libraries   (${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets)
    else ()
        message(WARNING "QT Not found! Skipping")
    endif ()
endif ()

### PLATFORM-SPECIFIC BLOCK ###
message (">> >> Platform-specific configuration")
if (UNIX)
    option(USE_COLORS "Use colorful output" ON) 
    if (USE_COLORS)
        add_compile_definitions (USE_COLORS)
    endif ()

    option(USE_X11 "Include X11 libraries and compile X11 implementation" ON)
    message(">> Finding X11")
    find_package(X11 QUIET COMPONENTS Xtst)
    if (USE_X11)
        if (X11_FOUND)
            add_compile_definitions (USE_X11)
            include_directories     (${X11_INCLUDE_DIR})
            link_directories        (${X11_LIBRARIES} ${X11_XTest_LIB})
            target_link_libraries   (${PROJECT_NAME} ${X11_LIBRARIES} ${X11_XTest_LIB})
        else ()
            message(WARNING "X11 Not found! Skipping")
        endif ()
    endif ()

    option(USE_WAYLAND "Include Wayland libraries and compile Wayland implementation" ON)
    message(">> Finding Wayland")
    find_library(WAYLAND_CLIENT_LIBRARY NAMES wayland-client libwayland-client)
    find_library(WAYLAND_SERVER_LIBRARY NAMES wayland-server libwayland-server)
    if (USE_WAYLAND)
        if (WAYLAND_CLIENT_LIBRARY AND WAYLAND_SERVER_LIBRARY)
            add_compile_definitions (USE_WAYLAND)
            target_link_libraries   (${PROJECT_NAME} ${WAYLAND_CLIENT_LIBRARY} ${WAYLAND_SERVER_LIBRARY})
        else ()
            message(WARNING "Wayland Not found! Skipping")
        endif ()
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

endif ()