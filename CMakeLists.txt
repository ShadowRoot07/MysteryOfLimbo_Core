cmake_minimum_required(VERSION 3.10)
project(MysteryOfLimbo)

set(CMAKE_CXX_STANDARD 17)

# Forzamos las rutas de inclusión para que busque en las carpetas del sistema 
# pero permitimos que el compilador de Android las use.
include_directories(
    include 
    include/gfx 
    src
    /usr/include/SDL2
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/elements/EarthSkill.cpp")

add_library(main SHARED ${SOURCES})

# En Android, estas librerías se cargan dinámicamente, 
# así que solo le decimos que "existen".
target_link_libraries(main 
    android
    log
    SDL2
    SDL2_mixer
)

