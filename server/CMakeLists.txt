cmake_minimum_required(VERSION 3.10)
project(JSONServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thread support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -pthread")

# Include directories
include_directories(inc)
include_directories(lib/inc)
include_directories(../inc)
include_directories(../ext)

# === Part З: Dynamic Library ===
# Library sources
set(LIB_SOURCES
    lib/src/json_handler_lib.cpp
    ../src/task.cpp
)

# Library headers
set(LIB_HEADERS
    lib/inc/json_handler_lib.h
    ../inc/task.h
)

# Create shared library with export macro
add_library(json_handler_lib SHARED ${LIB_SOURCES} ${LIB_HEADERS})
target_compile_definitions(json_handler_lib PRIVATE LIBRARY_EXPORT)

# Platform specific library settings
if(WIN32)
    target_compile_definitions(json_handler_lib PRIVATE _WIN32_WINNT=0x0601)
endif()

# === Server Executable ===
# Source files for the server executable
set(SERVER_SOURCES
    src/main_server.cpp
    src/server.cpp
)

# Header files
set(SERVER_HEADERS
    inc/server.h
)

# Create executable and link with library
add_executable(${PROJECT_NAME} ${SERVER_SOURCES} ${SERVER_HEADERS})
target_link_libraries(${PROJECT_NAME} json_handler_lib)

# Platform specific settings
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0601)
endif()

# Enable warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_options(json_handler_lib PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(json_handler_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy data file to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PROJECT_SOURCE_DIR}/../data/data.json
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/data.json
)