#pragma once

#include "Application.hpp"
#include "FastNoiseLite.h"
#include <random>
#include <time.h>
#include <thread>
#include <mutex>

class Triangle {
public:
    Triangle();
    Triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
    glm::vec3 points[3];
};
bool operator==(const Triangle& triangle1, const Triangle& triangle2);

struct Crater {
    glm::vec3 centre;
    float radius;
};

class Planet {
public:
    Planet(glm::vec3 pos, float size);
    ~Planet();

    glm::vec3 position;
    float size;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    int num_samples = 100; // 81608 triangles for 100 samples
    bool use_random_colors = false;
    bool use_noise = false;
    int noise_seed = 1;
    float noise_frequency = 0.3;
    float noise_strength = 5.0f;
    glm::vec3 base_color{0.0f, 0.5f, 0.9f};
    FastNoiseLite::NoiseType noise_type = FastNoiseLite::NoiseType_OpenSimplex2;
    FastNoiseLite::FractalType noise_fractal_type = FastNoiseLite::FractalType_FBm;
    
    int num_triangles = 0;
    int num_vertices = 0;

    std::vector<Crater> craters;
    int numCraters = 1;
    float rimWidth = 0.0f;
    float rimSteepness = 0.0f;
    float floorHeight = -10.0f;
    float craterDepth = -5;
    float smoothMin = 0.0f;
    float smoothMax = 0.0f;
    float craterMinSize = 0.05f;
    float craterMaxSize = 0.2f;
    float craterSizeBias = 0.0f;
    float cavityShape(float x);
    float rimShape(float x);
    float floorShape(float x);
    float craterShape(float x);

    void icosahedron();

    void proccessVertices(int startIdx, Triangle* triangles);
private:
    FastNoiseLite noise;

    glm::vec3 transformPoint(glm::vec3 point);

    float craterSizeFunc(float x, float bias);

    static const int numThreads = 4;
    std::thread threads[numThreads];
};

void proccessVerticesStatic(Planet* thisPrime, int startIdx, Triangle* triangles);

int sign(float value);
float smoothMinFunc(float a, float b, float k);
float clamp01(float a);