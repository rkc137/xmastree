cmake_minimum_required(VERSION 3.10)
project(xmastree LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)
set(SFML_STATIC_LIBRARIES TRUE)

add_executable(xmastree main.cpp)

target_link_libraries(xmastree PRIVATE SFML::Audio)
target_compile_features(xmastree PRIVATE cxx_std_17)

target_compile_definitions(xmastree PRIVATE SFML_STATIC)
target_link_options(xmastree PRIVATE -mwindows -static)