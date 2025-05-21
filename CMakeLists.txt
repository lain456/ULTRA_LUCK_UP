cmake_minimum_required(VERSION 3.10)
project(ULTRA_LUCK_UP C)

set(CMAKE_C_STANDARD 11)

# Find SDL and related libraries
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_mixer REQUIRED)

# Include directories
include_directories(
        ${SDL_INCLUDE_DIR}
        ${SDL_IMAGE_INCLUDE_DIR}
        ${SDL_TTF_INCLUDE_DIR}
        ${SDL_MIXER_INCLUDE_DIR}
        init
        code
)

# Add executable with all necessary source files
add_executable(ULTRA_LUCK_UP
        main.c
)

# Link libraries
target_link_libraries(ULTRA_LUCK_UP
        ${SDL_LIBRARY}
        ${SDL_IMAGE_LIBRARY}
        ${SDL_TTF_LIBRARY}
        ${SDL_MIXER_LIBRARY}
)
