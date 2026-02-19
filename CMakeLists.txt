cmake_minimum_required(VERSION 3.30)
project(test_http)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

add_executable(test_http

        src/Application.cpp
        src/Application.h

        src/assembler/JsonSpecializations.h
        src/assembler/StreamAssembler.h)
