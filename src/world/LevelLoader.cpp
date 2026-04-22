#include <vector>
#include <string>
#include <fstream>
#include <iostream> // Añadido para logs de error
#include "world/Platform.h"
#include "world/Enemy.h"

std::vector<Platform> LoadLevel(const std::string& path, std::vector<Enemy>& enemies) {
    std::vector<Platform> level;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de mapa en: " << path << std::endl;
        return level;
    }

    std::string line;
    int tileSize = 50; 
    bool parsingMap = false;
    int row = 0;

    enemies.clear();

    while (std::getline(file, line)) {
        // Eliminar posibles caracteres de retorno de carro (\r) de Windows
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        if (line.find("TILE_SIZE:") != std::string::npos) {
            try {
                tileSize = std::stoi(line.substr(11));
            } catch (...) {
                std::cerr << "Error leyendo TILE_SIZE, usando 50 por defecto." << std::endl;
                tileSize = 50;
            }
        } else if (line.find("MAP_START") != std::string::npos) {
            parsingMap = true;
            row = 0;
            continue;
        }

        if (parsingMap) {
            for (int col = 0; col < (int)line.length(); col++) {
                char c = line[col];
                if (c == ' ' || c == '.') continue; // Ignorar aire

                float x = (float)(col * tileSize);
                float y = (float)(row * tileSize);
                Rect r = {x, y, (float)tileSize, (float)tileSize};

                if (c == '#') {
                    level.push_back({r, NORMAL, 0});
                } else if (c == 'S') {
                    level.push_back({r, SPIKE, 25.0f});
                } else if (c == 'W' || c == 'V' || c == 'T') {
                    Enemy e;
                    // Centramos el enemigo en el tile de 50x50 para que no spawnee en la esquina
                    float offsetX = (tileSize - 32) / 2.0f;
                    float offsetY = (tileSize - 48); // Pegado al suelo del tile
                    
                    e.pos = {x + offsetX, y + offsetY};
                    e.hitbox = {e.pos.x, e.pos.y, 32, 48};
                    e.dir = 1;
                    e.health = (c == 'T') ? 100.0f : 50.0f; // La torreta tiene más vida
                    
                    if (c == 'W') e.type = WALKER;
                    else if (c == 'V') e.type = FLYER;
                    else e.type = TURRET;
                    
                    enemies.push_back(e);
                }
            }
            row++;
        }
    }
    
    file.close();
    std::cout << "Mapa cargado: " << level.size() << " plataformas, " << enemies.size() << " enemigos." << std::endl;
    return level;
}

