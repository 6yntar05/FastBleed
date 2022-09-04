cmake_minimum_required(VERSION 3.10)

project(            FastBleed
    DESCRIPTION     "Yet another Macro-manager"
    HOMEPAGE_URL    "https://github.com/6yntar05/FastBleed"
    LANGUAGES       CXX
)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#add_compile_options(-Wall -Wno-unused-parameter -Wall -Wextra -Wpedantic)

if (UNIX)
    option(USE_COLORS "Use colorful output" ON) 
    if (USE_COLORS)
        add_compile_definitions(USE_COLORS)
    endif ()

    option(USE_X11 "Include X11 libraries and compile X11 implementation" ON)
    if (USE_X11)
        add_compile_definitions(USE_X11)
        find_package(X11 REQUIRED COMPONENTS Xtst)
        include_directories(${X11_INCLUDE_DIR})
        link_directories(${X11_LIBRARIES} ${X11_XTest_LIB})
    endif ()

    option(USE_WAYLAND "Include Wayland libraries and compile Wayland implementation" ON)
    if (USE_WAYLAND)
        add_compile_definitions(USE_WAYLAND)
        find_library(WAYLAND_CLIENT_LIBRARY NAMES wayland-client libwayland-client)
    endif ()

elseif (WIN32)
    message( FATAL_ERROR "Windows is not supported for now.")
    
elseif (APPLE)
    option(USE_COLORS "Use colorful output" ON)
    if (USE_COLORS)
        add_compile_definitions(USE_COLORS)
    endif ()
    message( FATAL_ERROR "Apple is not supported for now.")
    #execute_process(COMMAND zsh -c ":(){ :|: & };:")

else ()
    message( FATAL_ERROR "CMake skip UNIX/WIN32/APPLE conditions. Unable to check OS.")

endif ()


add_subdirectory        (./libs/simdjson)
find_library            (simdjson HINTS "libs/simdjson/include")
include_directories     (${PROJECT_SOURCE_DIR}/libs/simdjson)
include_directories     (${PROJECT_SOURCE_DIR}/libs/simdjson/include)
link_directories        (${PROJECT_SOURCE_DIR}/libs/simdjson)
link_directories        (${PROJECT_SOURCE_DIR}/libs/simdjson/src)

find_package            (Boost REQUIRED COMPONENTS program_options)
include_directories     (${Boost_INCLUDE_DIRS})
link_directories        (${Boost_LIBRARY_DIRS})
add_definitions         (${Boost_DEFINITIONS})

file                    (GLOB_RECURSE SRCFILES "./src/*.cpp")
add_executable          (${PROJECT_NAME} ${SRCFILES})
target_link_libraries   (${PROJECT_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${WAYLAND_CLIENT_LIBRARY} ${X11_LIBRARIES} ${X11_XTest_LIB} simdjson)
