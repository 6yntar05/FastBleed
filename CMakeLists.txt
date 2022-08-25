cmake_minimum_required(VERSION 3.10)

project(            FastBleed
    DESCRIPTION     "Yet another crossplatform clicker"
    HOMEPAGE_URL    "https://github.com/6yntar05/FastBleed"
    LANGUAGES       CXX
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -lwayland-client")
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
    message("Apple is not supported.")
else()
    message("WTF")
endif()

include_directories     (${Boost_INCLUDE_DIRS})
link_directories        (${Boost_LIBRARY_DIRS})
add_definitions         (${Boost_DEFINITIONS})

file                    (GLOB_RECURSE SRCFILES "./platform/*.cpp" "./ui/*.cpp")
add_executable          (${PROJECT_NAME} ./FastBleed.cpp ${SRCFILES})
target_link_libraries   (${PROJECT_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${X11_LIBRARIES} ${X11_XTest_LIB})
