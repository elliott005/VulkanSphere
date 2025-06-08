#include "Planet.hpp"

Planet::Planet(glm::vec3 pos, float size) {
    this->position = pos;
    this->size = size;
    this->fibonacciSphere(100);
}

Planet::~Planet() {

}

void Planet::fibonacciSphere(int samples) {
    float phi = M_PI * (sqrt(5.0f) - 1.0f);  // golden angle in radians

    for (int i = 0; i < samples; i++) {
        float y = 1 - (i / float(samples - 1)) * 2;  // y goes from 1 to -1
        float radius = sqrt(1 - y * y); //radius at y

        float theta = phi * i;  // golden angle increment

        float x = cos(theta) * radius;
        float z = sin(theta) * radius;

        this->vertices.push_back({{x * this->size, y * this->size, z * this->size}, {1.0f, 0.0f, 0.0f}});
        unsigned short j = i;
        if (i > 1) {
            this->indices.push_back(j - 2);
            this->indices.push_back(j - 1);
            this->indices.push_back(j);
        }
    }
}