cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -lXtst")

project(            FastBleed
    DESCRIPTION     "Yet another crossplatform clicker"
    HOMEPAGE_URL    "https://github.com/6yntar05/FastBleed"
    LANGUAGES       CXX
)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(X11 REQUIRED COMPONENTS Xtst)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

include_directories     (${Boost_INCLUDE_DIRS} ${X11_INCLUDE_DIR})
link_directories        (${Boost_LIBRARY_DIRS} ${X11_LIBRARIES})
add_definitions         (${Boost_DEFINITIONS})
#add_definitions        (-std=c++11)

file                    (GLOB_RECURSE SRCFILES "./platform/*.cpp")
add_executable          (${PROJECT_NAME} ./FastBleed.cpp ${SRCFILES})
target_link_libraries   (${PROJECT_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${X11_LIBRARIES})