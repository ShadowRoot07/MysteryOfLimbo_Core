CXX = clang++
# Agregamos todas las subcarpetas de include
CXXFLAGS = -Wall -std=c++17 -Iinclude -Iinclude/input -Iinclude/physics -Iinclude/player -Iinclude/world \
           $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)

# Busca todos los .cpp en cualquier subcarpeta de src
SRC = $(shell find src -name "*.cpp")
OBJ = $(SRC:src/%.cpp=build/%.o)
TARGET = limbo_core

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Regla para compilar objetos manteniendo la estructura de carpetas
build/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

