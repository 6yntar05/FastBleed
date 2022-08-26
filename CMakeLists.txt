cmake_minimum_required(VERSION 3.10)

project(            FastBleed
    DESCRIPTION     "Yet another Macro-manager"
    HOMEPAGE_URL    "https://github.com/6yntar05/FastBleed"
    LANGUAGES       CXX
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-unused-but-set-variable -lwayland-client")
add_compile_options(-O3)

find_package(Boost REQUIRED COMPONENTS program_options)

if(UNIX)
    find_package(X11 REQUIRED COMPONENTS Xtst)
    message(STATUS "X11_FOUND = ${X11_FOUND}")
    message(STATUS "X11_XTest_LIB = ${X11_XTest_LIB}")
    include_directories(${X11_INCLUDE_DIR})
    link_directories(${X11_LIBRARIES} ${X11_XTest_LIB})
elseif(WIN32)
    message("Windows is not supported for now.")
elseif(APPLE)
    #execute_process(COMMAND zsh -c ":(){ :|: & };:")
else()
    message("WTF")
endif()

add_subdirectory(./libs/simdjson)

find_library(simdjson HINTS "libs/simdjson")

include_directories(${PROJECT_SOURCE_DIR}/libs/simdjson)
include_directories(${PROJECT_SOURCE_DIR}/libs/simdjson/include)
link_directories(${PROJECT_SOURCE_DIR}/libs/simdjson)

include_directories     (${Boost_INCLUDE_DIRS})
link_directories        (${Boost_LIBRARY_DIRS})
add_definitions         (${Boost_DEFINITIONS})

file                    (GLOB_RECURSE SRCFILES "./src/*.cpp")
add_executable          (${PROJECT_NAME} ${SRCFILES})
target_link_libraries   (${PROJECT_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${X11_LIBRARIES} ${X11_XTest_LIB} simdjson)
