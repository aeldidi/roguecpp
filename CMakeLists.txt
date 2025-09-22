cmake_minimum_required(VERSION 3.30)
project(rogue CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
include_directories(
    tracy/public
)

option(TRACY_ENABLE "Enable Tracy Profiler" ON)
add_subdirectory(tracy)

find_package(SDL3 QUIET)
find_package(SDL3_image QUIET)
if (NOT SDL3_FOUND AND NOT SDL3_image_FOUND)
    message(STATUS "Getting SDL3 from Github")
    FetchContent_Declare(SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG        release-3.2.22
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(SDL3)

    FetchContent_Declare(SDL3_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-3.2.4  # match SDL3 version
    )
    FetchContent_MakeAvailable(SDL3_image)
else()
    message(STATUS "Using local SDL3")
endif()

add_executable(rogue WIN32
    tracy/public/TracyClient.cpp
    src/main.cpp
    src/Texture.cpp
    src/prelude/panic.cpp
    src/prelude/Threadpool.cpp
    src/Game.cpp)

# add_compile_options(-fsanitize=address,undefined)
# add_link_options(-fsanitize=address,undefined)

target_include_directories(rogue PUBLIC ${SDL3_INCLUDE_DIRS})

target_compile_definitions(rogue PRIVATE SDL_MAIN_USE_CALLBACKS)

target_link_libraries(rogue
    PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        Tracy::TracyClient)

if (WIN32)
    add_custom_command(
            TARGET rogue POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:rogue>"
            VERBATIM
    )
endif()
