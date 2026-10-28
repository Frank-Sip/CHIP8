cmake_minimum_required(VERSION 3.20)

project(CHIP8Emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL3
add_subdirectory(SDL EXCLUDE_FROM_ALL)

# Executable
add_executable(${PROJECT_NAME}
    main.cpp
)

# Link SDL3
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Copy SDL3.dll to the executable directory
if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3-shared>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM
    )
endif()