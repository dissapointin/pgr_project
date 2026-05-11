#include "config.h"
#include "render.h"
#include "camera.h"
#include "fan.h"
#include <fstream>
#include <sstream>
#include <iostream>

extern glm::vec3 succulentPos;
extern float succulentBaseY;

// fog values stored globally for shaders is config is not loaded
/// @brief Fog start distance loaded from config file
float configFogStart = 1.0f; // fog starts 1 from the camera

/// @brief Fog end distance loaded from config file
float configFogEnd = 10.0f; // fog ends 10 from the camera (and the most color)

void loadConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open config: " << path << std::endl;
        return;
    }

    int validCount = 0;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string key;
        ss >> key;

        if (key == "fogStart") {
            float v; ss >> v;
            if (v >= 0.0f && v < 100.0f) { configFogStart = v; validCount++; }
            else std::cerr << "Invalid fogStart: " << v << std::endl;
        }
        else if (key == "fogEnd") {
            float v; ss >> v;
            if (v > 0.0f && v <= 100.0f) { configFogEnd = v; validCount++; }
            else std::cerr << "Invalid fogEnd: " << v << std::endl;
        }
        else if (key == "succulentX") {
            float v; ss >> v;
            if (v >= -8.0f && v <= 8.0f) { succulentPos.x = v; validCount++; }
            else std::cerr << "Invalid succulentX: " << v << std::endl;
        }
        else if (key == "succulentY") {
            float v; ss >> v;
            if (v >= -4.0f && v <= 4.0f) {
                succulentPos.y = v;
                succulentBaseY = v;
                validCount++;
            }
            else std::cerr << "Invalid succulentY: " << v << std::endl;
        }
        else if (key == "succulentZ") {
            float v; ss >> v;
            if (v >= -10.0f && v <= 10.0f) { succulentPos.z = v; validCount++; }
            else std::cerr << "Invalid succulentZ: " << v << std::endl;
        }
        else if (key == "catmull0") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[0] = glm::vec3(x, y, z); validCount++;
        }
        else if (key == "catmull1") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[1] = glm::vec3(x, y, z); validCount++;
        }
        else if (key == "catmull2") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[2] = glm::vec3(x, y, z); validCount++;
        }
        else if (key == "catmull3") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[3] = glm::vec3(x, y, z); validCount++;
        }
        else if (key == "catmull4") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[4] = glm::vec3(x, y, z); validCount++;
        }
        else if (key == "catmull5") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[5] = glm::vec3(x, y, z); validCount++;
        }
        else if (key == "catmull6") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[6] = glm::vec3(x, y, z); validCount++;
        }
        else if (key == "catmull7") {
            float x, y, z; ss >> x >> y >> z;
            catmullPoints[7] = glm::vec3(x, y, z); validCount++;
        }
    }
    file.close();
    std::cout << "Config loaded: " << validCount << " valid values from " << path << std::endl;
}