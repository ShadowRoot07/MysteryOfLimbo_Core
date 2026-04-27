cmake_minimum_required(VERSION 3.10)
project(MysteryOfLimbo)

set(CMAKE_CXX_STANDARD 17)

# Cargamos el módulo para encontrar librerías
find_package(PkgConfig REQUIRED)

# Buscamos SDL2 y SDL2_mixer en el sistema de GitHub
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

# Incluimos tus carpetas Y las que nos diga pkg-config
include_directories(
    include 
    include/gfx 
    src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/elements/EarthSkill.cpp")

add_library(main SHARED ${SOURCES})

# Enlazamos usando las rutas absolutas que pkg-config encontró
target_link_libraries(main 
    android
    log
    ${SDL2_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

