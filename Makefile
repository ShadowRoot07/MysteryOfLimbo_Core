# Mystery of Limbo Makefile
CXX = g++
CXXFLAGS = -Iinclude -Isrc `sdl2-config --cflags` -std=c++17 -MMD
LDFLAGS = `sdl2-config --libs`

# Directorios
SRC_DIR = src
BUILD_DIR = build

# Encontrar todos los .cpp excepto los de elements (porque se incluyen en main o se compilan aparte)
# Nota: EarthSkill.cpp se incluye en main.cpp, así que no lo compilamos individualmente para evitar símbolos duplicados
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp' ! -name 'EarthSkill.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS = $(OBJECTS:.o=.d)

TARGET = limbo_core

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Crear directorios de build automáticamente
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

