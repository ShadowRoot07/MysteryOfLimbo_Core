cmake_minimum_required(VERSION 3.16)

project(MysteryOfLimbo VERSION 1.0)

# Requerir C++17 como en tu Makefile
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar paquetes de SDL2 (en Android se cargan como librerías externas)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Directorios de inclusión (replicando tus CXXFLAGS)
include_directories(
    include 
    include/gfx 
    src
)

# Encontrar todos los .cpp recursivamente (como tu $(shell find ...))
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Excluir EarthSkill.cpp para evitar duplicación de símbolos (como en tu Makefile)
list(FILTER SOURCES EXCLUDE REGEX "src/elements/EarthSkill.cpp")

# Crear la librería compartida que Android necesita
add_library(main SHARED ${SOURCES})

# Vincular dependencias (equivalente a LDFLAGS)
target_link_libraries(main 
    SDL2::SDL2 
    SDL2_mixer::SDL2_mixer
    android 
    log
)

