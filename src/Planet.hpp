#pragma once

#include "Application.hpp"
#include <random>
#include <unordered_set>
#include <unordered_map>

class Triangle {
public:
    Triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
    glm::vec3 points[3];
};
bool operator==(const Triangle& triangle1, const Triangle& triangle2);

class Planet {
public:
    Planet(glm::vec3 pos, float size);
    ~Planet();

    glm::vec3 position;
    float size;

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    std::unordered_set<glm::vec3> points;
    std::unordered_map<glm::vec3, int> pointsIndexes;
private:
    int num_samples = 100;
    glm::vec3 color{0.5f, 0.5f, 0.0f};

    void icosahedron();
};

int sign(float value);