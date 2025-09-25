cmake_minimum_required(VERSION 3.10)
project(JSONHandlerTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(../inc)
include_directories(../ext)

# Test sources
set(TEST_SOURCES
    test_json_handler.cpp
    ../src/task.cpp
)

# Create test executable
add_executable(test_runner ${TEST_SOURCES})

# Enable coverage if requested (Part И)
option(ENABLE_COVERAGE "Enable code coverage reporting" ON)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Add coverage flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

        # Find lcov
        find_program(LCOV lcov)
        find_program(GENHTML genhtml)

        if(LCOV AND GENHTML)
            # Add coverage target
            add_custom_target(coverage
                COMMAND ${CMAKE_COMMAND} -E remove_directory coverage
                COMMAND ${CMAKE_COMMAND} -E make_directory coverage
                COMMAND ${LCOV} --capture --initial --directory . --output-file coverage/base.info
                COMMAND test_runner
                COMMAND ${LCOV} --capture --directory . --output-file coverage/test.info
                COMMAND ${LCOV} --add-tracefile coverage/base.info --add-tracefile coverage/test.info --output-file coverage/total.info
                COMMAND ${LCOV} --remove coverage/total.info '*/ext/*' '*/tests/*' '/usr/*' --output-file coverage/filtered.info
                COMMAND ${GENHTML} coverage/filtered.info --output-directory coverage/html
                COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated in coverage/html/index.html"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                DEPENDS test_runner
            )
        else()
            message(WARNING "lcov/genhtml not found, coverage target will not be available")
        endif()
    else()
        message(WARNING "Coverage is only supported with GCC or Clang")
    endif()
endif()

# Platform specific settings
if(WIN32)
    target_compile_definitions(test_runner PRIVATE _WIN32_WINNT=0x0601)
endif()

# Copy test data
add_custom_command(TARGET test_runner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PROJECT_SOURCE_DIR}/../data/data.json
    $<TARGET_FILE_DIR:test_runner>/data.json
)