cmake_minimum_required(VERSION 3.10)
project(MysteryOfLimbo)

set(CMAKE_CXX_STANDARD 17)

# En lugar de find_package que falla, definimos las rutas manualmente
# El NDK ya sabe buscar en los directorios de inclusión estándar
include_directories(include include/gfx src)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/elements/EarthSkill.cpp")

# Creamos la librería
add_library(main SHARED ${SOURCES})

# Vinculamos con los nombres de las librerías directamente
# En Android, SDL2 y SDL2_mixer se inyectan en tiempo de ejecución
target_link_libraries(main 
    android
    log
    SDL2
    SDL2_mixer
)

