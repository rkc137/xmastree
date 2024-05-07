cmake_minimum_required(VERSION 3.10)
project(xmastree LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(SFML_STATIC_LIBRARIES TRUE)

add_executable(xmastree main.cpp)

target_link_libraries(xmastree PRIVATE sfml-audio)
target_compile_features(xmastree PRIVATE cxx_std_17)