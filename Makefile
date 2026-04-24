# Mystery of Limbo Makefile
CXX = g++
# Añadido -Iinclude/gfx para la nueva librería
CXXFLAGS = -Iinclude -Iinclude/gfx -Isrc `sdl2-config --cflags` -std=c++17 -MMD
LDFLAGS = `sdl2-config --libs`

# Directorios
SRC_DIR = src
BUILD_DIR = build

# Encontrar todos los .cpp excepto los de elements
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp' ! -name 'EarthSkill.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS = $(OBJECTS:.o=.d)                                    
TARGET = limbo_core

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

