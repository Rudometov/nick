cmake_minimum_required(VERSION 3.10)
project(JSONHandler)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Thread support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -pthread")

# Include directories
include_directories(inc)
include_directories(ext)

# Source files
set(SOURCES
    src/main.cpp
    src/task.cpp
)

# Header files
set(HEADERS
    inc/task.h
)

# Create standalone executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Copy data file to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PROJECT_SOURCE_DIR}/data/data.json
    ${OUTPUT_DIR}/data.json
)

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

# Add subdirectories for client-server components
add_subdirectory(client)
add_subdirectory(server)

# Part И: Add tests subdirectory if tests exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
endif()

# Part К: Doxygen documentation generation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyconf)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doc)

    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUT}
        COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/doc && ${DOXYGEN_EXECUTABLE} doxyconf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found, documentation target will not be available")
endif()