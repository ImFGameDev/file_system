cmake_minimum_required(VERSION 3.30)
project(file_system VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        debug_system
        GIT_REPOSITORY git@github.com:ImFGameDev/debug_system.git
        GIT_TAG main
)

FetchContent_MakeAvailable(debug_system)

add_library(${PROJECT_NAME}
        src/application.cpp
        src/stream_assembler.cpp
        src/json_specializations.cpp
)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC debug_system)
