cmake_minimum_required(VERSION 3.10)
project(JSONClient)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thread support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -pthread")

# Include directories
include_directories(inc)
include_directories(../inc)
include_directories(../ext)

# Source files
set(CLIENT_SOURCES
    src/main_client.cpp
    src/client.cpp
)

# Header files
set(CLIENT_HEADERS
    inc/client.h
)

# Create executable
add_executable(${PROJECT_NAME} ${CLIENT_SOURCES} ${CLIENT_HEADERS})

# Platform specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0601)
endif()

# Enable warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()