cmake_minimum_required(VERSION 3.10)
project(JSONServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thread support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -pthread")

# Include directories
include_directories(inc)
include_directories(../inc)
include_directories(../ext)

# Source files for the library
set(LIB_SOURCES
    ../src/task.cpp
)

# Source files for the server executable
set(SERVER_SOURCES
    src/main_server.cpp
    src/server.cpp
)

# Header files
set(HEADERS
    inc/server.h
    ../inc/task.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SERVER_SOURCES} ${LIB_SOURCES} ${HEADERS})

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

# Copy data file to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PROJECT_SOURCE_DIR}/../data/data.json
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/data.json
)