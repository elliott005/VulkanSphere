#pragma once

#include "Application.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

class Planet {
public:
    Planet(glm::vec3 pos, float size);
    ~Planet();

    glm::vec3 position;
    float size;

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
private:
    void fibonacciSphere(int samples);
};