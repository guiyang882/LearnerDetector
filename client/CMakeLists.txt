cmake_minimum_required(VERSION 3.6)
project(client)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        client.cpp)
add_executable(client ${SOURCE_FILES})
