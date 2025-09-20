cmake_minimum_required(VERSION 3.30)
project(rogue CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
    message(STATUS "Getting SDL3 from Github")
    FetchContent_Declare(SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG        release-3.2.18
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )

    FetchContent_MakeAvailable(SDL3)
else()
    message(STATUS "Using local SDL3")
endif()

add_executable(rogue WIN32 src/main.cpp)

target_include_directories(rogue PUBLIC ${SDL3_INCLUDE_DIRS})

target_compile_definitions(rogue PRIVATE SDL_MAIN_USE_CALLBACKS)

target_link_libraries(rogue PRIVATE SDL3::SDL3)

if (WIN32)
    add_custom_command(
            TARGET rogue POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:rogue>"
            VERBATIM
    )
endif()
