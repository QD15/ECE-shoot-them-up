cmake_minimum_required(VERSION 3.10)
project(Projetttttt_V2 C)

set(CMAKE_C_STANDARD 99)

# Chemins MSYS2/UCRT64
set(MSYS2_PATH "C:/msys64/ucrt64")

include_directories(${MSYS2_PATH}/include)
link_directories(${MSYS2_PATH}/lib)

# Sources
add_executable(game
        main.c
        game.c
        player.c
        enemy.c
        input.c
)

# Bibliotheques Allegro (allegro_ttf ajoute pour les polices de taille variable)
target_link_libraries(game
        allegro
        allegro_primitives
        allegro_font
        allegro_ttf
        allegro_image
        allegro_audio
        allegro_acodec
)
