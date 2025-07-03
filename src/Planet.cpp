#include "Planet.hpp"

Planet::Planet(glm::vec3 pos, float size) {
    this->position = pos;
    this->size = size;
    
    this->icosahedron();
}

Planet::~Planet() {

}

void Planet::icosahedron() {
    this->indices.clear();
    this->vertices.clear();
    this->uniquePoints.clear();
    this->transformedPoints.clear();

    this->noise.SetNoiseType(this->noise_type);
    this->noise.SetFrequency(this->noise_frequency);
    this->noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    this->noise.SetSeed(static_cast<unsigned int>(time(nullptr)));

    printf("num samples: %i\n", this->num_samples);

    const float t = (1.0f + sqrt(5.0f)) / 2.0f;

    // 12 unique, normalized vertices
    std::vector<glm::vec3> verts = {
        glm::normalize(glm::vec3(-1,  t,  0)),
        glm::normalize(glm::vec3( 1,  t,  0)),
        glm::normalize(glm::vec3(-1, -t,  0)),
        glm::normalize(glm::vec3( 1, -t,  0)),
        glm::normalize(glm::vec3( 0, -1,  t)),
        glm::normalize(glm::vec3( 0,  1,  t)),
        glm::normalize(glm::vec3( 0, -1, -t)),
        glm::normalize(glm::vec3( 0,  1, -t)),
        glm::normalize(glm::vec3( t,  0, -1)),
        glm::normalize(glm::vec3( t,  0,  1)),
        glm::normalize(glm::vec3(-t,  0, -1)),
        glm::normalize(glm::vec3(-t,  0,  1))
    };

    // 20 triangles, each referencing the above vertices by index
    std::vector<Triangle> startTriangles = {
        {verts[0], verts[11], verts[5]},
        {verts[0], verts[5], verts[1]},
        {verts[0], verts[1], verts[7]},
        {verts[0], verts[7], verts[10]},
        {verts[0], verts[10], verts[11]},
        {verts[1], verts[5], verts[9]},
        {verts[5], verts[11], verts[4]},
        {verts[11], verts[10], verts[2]},
        {verts[10], verts[7], verts[6]},
        {verts[7], verts[1], verts[8]},
        {verts[3], verts[9], verts[4]},
        {verts[3], verts[4], verts[2]},
        {verts[3], verts[2], verts[6]},
        {verts[3], verts[6], verts[8]},
        {verts[3], verts[8], verts[9]},
        {verts[4], verts[9], verts[5]},
        {verts[2], verts[4], verts[11]},
        {verts[6], verts[2], verts[10]},
        {verts[8], verts[6], verts[7]},
        {verts[9], verts[8], verts[1]}
    };

    /* int s = 1;
    std::vector<Triangle> startTriangles = {
        {glm::vec3(s, 0, -s),  glm::vec3(0, s, 0),   glm::vec3(-s, 0, -s)},  // top front
        {glm::vec3(-s, 0, s),  glm::vec3(-s, 0, -s), glm::vec3(0, s, 0)},  // top left
        {glm::vec3(-s, 0, s),  glm::vec3(0, s, 0),   glm::vec3(s, 0, s)},  // top back
        {glm::vec3(s, 0, -s),  glm::vec3(s, 0, s),   glm::vec3(0, s, 0)},  // top right
        {glm::vec3(-s, 0, -s), glm::vec3(0, -s, 0),  glm::vec3(s, 0, -s)}, // bottom front
        {glm::vec3(-s, 0, -s), glm::vec3(-s, 0, s),  glm::vec3(0, -s, 0)}, // bottom left
        {glm::vec3(s, 0, s),   glm::vec3(0, -s, 0),  glm::vec3(-s, 0, s)}, // bottom back
        {glm::vec3(s, 0, s),   glm::vec3(s, 0, -s),  glm::vec3(0, -s, 0)}, // bottom right
    }; */

    std::vector<Triangle> triangles;
    for (Triangle& triangle : startTriangles) {

        std::vector<glm::vec3> triEdgePoints((num_samples + 2) * 2);
        for (int i = 0; i <= num_samples + 1; i++) {
            triEdgePoints[i] = triangle.points[0] + 1.0f / (num_samples + 1) * i * (triangle.points[1] - triangle.points[0]);
            triEdgePoints[i + num_samples + 2] = triangle.points[0] + 1.0f / (num_samples + 1) * i * (triangle.points[2] - triangle.points[0]);
        }
        
        std::vector<glm::vec3> triPoints;
        for (int y = 0; y <= num_samples + 1; y++) {
            for (int x = 0; x < y + 1; x++) {
                glm::vec3 v0 = triEdgePoints[y];
                glm::vec3 v1 = triEdgePoints[y + num_samples + 2];
                if (y == 0) {
                    triPoints.push_back(v0);
                } else {
                    triPoints.push_back(v0 + 1.0f / y * x * (v1 - v0));
                }
            }
        }
        
        /* if (triangle == startTriangles[0]) {
            for (glm::vec3 point : triPoints) {
                printf("triPoint: %f, %f, %f\n", point.x, point.y, point.z);
            }
        } */

        for (int i = 0; i < num_samples + 1; i++) {
            int numTri = i * 2 + 1;
            for (int j = 0; j < numTri; j++) {
                if (j % 2 == 0) {
                    glm::vec3 v3 = triPoints[(i * (i + 1)) / 2 + j / 2];
                    glm::vec3 v4 = triPoints[((i + 1) * (i + 2)) / 2 + j / 2];
                    glm::vec3 v5 = triPoints[((i + 1) * (i + 2)) / 2 + j / 2 + 1];
    
                    triangles.push_back({v3, v4, v5});
                } else {
                    glm::vec3 v3 = triPoints[(i * (i + 1)) / 2 + j / 2];
                    glm::vec3 v4 = triPoints[((i + 1) * (i + 2)) / 2 + j / 2 + 1];
                    glm::vec3 v5 = triPoints[(i * (i + 1)) / 2 + j / 2 + 1];
    
                    triangles.push_back({v3, v4, v5});
                }
            }
        }
    }

    printf("num triangles: %i\n", triangles.size());

    //int count = 0;
    for (Triangle& triangle : triangles) {
        for (int i = 0; i < 3; i++) {
            /* if (count > 40300) {
                } */
            //printf("point %i: %f, %f, %f\n", i, triangle.points[i].x, triangle.points[i].y, triangle.points[i].z);
            glm::vec3 pointKey = triangle.points[i];
            auto it = this->uniquePoints.find(pointKey);
            if (it == this->uniquePoints.end()) {
                this->uniquePoints.insert(pointKey);
                this->transformedPoints[pointKey] = this->transformPoint(triangle.points[i]);
            }
            /* glm::vec3 point = this->transformedPoints[pointKey];
            this->indices.push_back(this->vertices.size());
            this->vertices.push_back({point, color}); */
            //printf("%i\n", points[i]);
        }

        glm::vec3 v0 = this->transformedPoints[triangle.points[0]];
        glm::vec3 v1 = this->transformedPoints[triangle.points[1]];
        glm::vec3 v2 = this->transformedPoints[triangle.points[2]];
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        //printf("normal: %f, %f, %f\n", normal.x, normal.y, normal.z);
        glm::vec3 color;
        if (use_random_colors) {
            color = glm::vec3(static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
        } else {
            color = base_color * (normal.y + 1.0f) / 2.0f;//glm::abs(normal);//(normal + glm::vec3(1.0f)) / 2.0f;
        }

        for (int i = 0; i < 3; i++) {
            /* if (count > 40300) {
                } */
            //printf("point %i: %f, %f, %f\n", i, triangle.points[i].x, triangle.points[i].y, triangle.points[i].z);
            glm::vec3 pointKey = triangle.points[i];
            glm::vec3 point = this->transformedPoints[pointKey];
            this->indices.push_back(this->vertices.size());
            this->vertices.push_back({point, color});
            //printf("%i\n", points[i]);
        }
        //count++;
    }

    //printf("vertices max size: %i\n", this->vertices.max_size());
    printf("vertices: %i\n", this->vertices.size());
    printf("unique points: %i\n", this->uniquePoints.size());
}

glm::vec3 Planet::transformPoint(glm::vec3 point) {
    if (this->use_noise) {
        float noiseValue = this->noise.GetNoise(point.x + 10.0f, point.y + 10.0f, point.z + 10.0f);
        noiseValue = (noiseValue + 1.0f) / 2.0f; // Normalize to [0, 1]
        noiseValue *= this->noise_strength;
        //noiseValue = noiseValue * 0.5f + 1.0f;
        return this->position + glm::normalize(point) * this->size + glm::normalize(point) * noiseValue;// * (static_cast<float>(rand()) / RAND_MAX / 2 + 1);
    }
    return this->position + glm::normalize(point) * this->size;
}

Triangle::Triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
    this->points[0] = point1;
    this->points[1] = point2;
    this->points[2] = point3;
}

bool operator==(const Triangle& triangle1, const Triangle& triangle2) {
    return (triangle1.points[0] == triangle2.points[0] && triangle1.points[1] == triangle2.points[1] && triangle1.points[2] == triangle2.points[2]);
}

int sign(float value) {
    return (value > 0) - (value < 0);
}