CXX = clang++
# -MMD genera dependencias para que si cambias un .h sepa que recompilar el .cpp
CXXFLAGS = -Wall -std=c++17 -MMD -Iinclude -Iinclude/input -Iinclude/physics -Iinclude/player -Iinclude/world \
           $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf)

SRC = $(shell find src -name "*.cpp")
OBJ = $(SRC:src/%.cpp=build/%.o)
DEP = $(OBJ:.o=.d)
TARGET = limbo_core

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Incluimos las dependencias generadas
-include $(DEP)

clean:
	rm -rf build $(TARGET)

