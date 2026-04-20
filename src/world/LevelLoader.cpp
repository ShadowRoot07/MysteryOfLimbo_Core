#include <vector>
#include <string>
#include <fstream>
#include "world/Platform.h"

std::vector<Platform> LoadLevel(const std::string& path) {
    std::vector<Platform> level;
    std::ifstream file(path);
    std::string line;
    int tileSize = 50;
    bool parsingMap = false;
    int row = 0;

    while (std::getline(file, line)) {
        if (line.find("TILE_SIZE:") != std::string::npos) {
            tileSize = std::stoi(line.substr(11));
        } else if (line.find("MAP_START") != std::string::npos) {
            parsingMap = true;
            continue;
        }

        if (parsingMap) {
            for (int col = 0; col < (int)line.length(); col++) {
                char c = line[col];
                Rect r = {(float)(col * tileSize), (float)(row * tileSize), (float)tileSize, (float)tileSize};
                if (c == '#') level.push_back({r, NORMAL, 0});
                else if (c == 'S') level.push_back({r, SPIKE, 25.0f});
            }
            row++;
        }
    }
    return level;
}

