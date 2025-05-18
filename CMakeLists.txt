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
        code
        init/init.c
        code/game/game.c
        code/button/button.c
        code/text/txt.c
        code/menu/menu.c
        code/load_menus/load_menus.c
        code/slider/slider.c
        code/tools/tools.c
        code/audio/audio.c
        code/Background/background.c
        code/Player/player.c
        code/arduino/arduino.c
        shity_code/shity_code.c
        shity_code/shity_code.h
        code/input/input.c
        code/input/input.h
        code/my_input/my_input.c
        code/my_input/my_input.h  # Add this line if it's missing
)

# Link libraries
target_link_libraries(ULTRA_LUCK_UP
        ${SDL_LIBRARY}
        ${SDL_IMAGE_LIBRARY}
        ${SDL_TTF_LIBRARY}
        ${SDL_MIXER_LIBRARY}
)