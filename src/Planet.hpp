#pragma once

#include "Application.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

class Edge {
    public:
    Edge();
    Edge(glm::vec3 point1, glm::vec3 point2);
    glm::vec3 points[2];
};

bool areEdgesEqual(const Edge& edge1, const Edge& edge2);

class Triangle {
public:
    Triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
    bool pointIsInside(glm::vec3 point);
    glm::vec3 points[3];
    Edge edges[3];
    glm::vec3 center;
    float size = -1;

};
bool operator==(const Triangle& triangle1, const Triangle& triangle2);

bool isEdgeShared(Edge edge, const Triangle& current, std::vector<Triangle> triangles);

class Planet {
public:
    Planet(glm::vec3 pos, float size);
    ~Planet();

    glm::vec3 position;
    float size;

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    std::vector<glm::vec3> points;
private:
    glm::vec3 color{0.5f, 0.5f, 0.0f};

    void fibonacciSphere(int samples);
    void bowyer_watson();
};