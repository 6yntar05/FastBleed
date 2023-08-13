cmake_minimum_required(VERSION 3.10)
project( FastBleed
    DESCRIPTION     "Yet another clicker"
    HOMEPAGE_URL    "https://github.com/6yntar05/FastBleed"
    LANGUAGES       CXX
)
include("./cmake/compiller_props.cmake")
include("./cmake/submodules.cmake")

### COMPILE PROPERTIES ###
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### OPTIONS ###
option(USE_Compiler_PROPERTIES "Use cringe compiler propertires" ON)
if(${USE_Compiler_PROPERTIES})
    SetCompilerProperties()
endif()

### SUBMODULES ###
submodules_update()

### SRC FILES ###
# Find all src files except "platform"
file(GLOB SRCFILES "./src/*.cpp")
file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/utils/*.cpp" "./src/ui/*")

if (UNIX) # Compile-time platform selection
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/unix/*.cpp")
elseif (WIN32)
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/winapi/*.cpp")
elseif (APPLE)
    file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/platform/apple/*.cpp")
endif()
file(GLOB_RECURSE SRCFILES ${SRCFILES} "./src/ui/*.h")

include_directories(AFTER SYSTEM ./src)
include_directories(AFTER SYSTEM ./libs/protocols)
add_executable(${PROJECT_NAME} ${SRCFILES})
target_include_directories(${PROJECT_NAME} PRIVATE ./src)

### SIMDJSON ###
add_subdirectory        (./libs/simdjson)
find_library            (simdjson HINTS "libs/simdjson/include")
include_directories     (${PROJECT_SOURCE_DIR}/libs/simdjson/include)
link_directories        (${PROJECT_SOURCE_DIR}/libs/simdjson/src)
target_link_libraries   (${PROJECT_NAME} simdjson)

### SPDLOG ###
add_subdirectory        (./libs/spdlog)
find_library            (spdlog HINTS "libs/spdlog/include")
include_directories     (${PROJECT_SOURCE_DIR}/libs/spdlog/include)
link_directories        (${PROJECT_SOURCE_DIR}/libs/spdlog/src)
target_link_libraries   (${PROJECT_NAME} spdlog)

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

    if (LINUX)
        option(USE_UINPUT "Compile uinput implementation" ON)
        if (USE_UINPUT)
            message(">> Finding Uinput deps")
            find_package(Libinput QUIET)
            find_library(UDEV_LIBRARY NAMES udev)
            if (Libinput_LIBRARY AND UDEV_LIBRARY)
                add_compile_definitions (USE_UINPUT)
                target_link_libraries   (${PROJECT_NAME} ${Libinput_LIBRARY} ${UDEV_LIBRARY})
            else ()
                message(WARNING "Libinput or Udev Not found! Skipping")
            endif ()
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
