#include "Planet.hpp"

Planet::Planet(glm::vec3 pos, float size) {
    this->position = pos;
    this->size = size;
    
    this->icosahedron();
}

Planet::~Planet() {

}

void Planet::icosahedron() {
    int s = 1;
    std::vector<Triangle> startTriangles = {
        {glm::vec3(-s, 0, -s), glm::vec3(s, 0, -s), glm::vec3(0, s, 0)},  // top front
        {glm::vec3(-s, 0, -s), glm::vec3(-s, 0, s), glm::vec3(0, s, 0)},  // top left
        {glm::vec3(s, 0, s),   glm::vec3(-s, 0, s), glm::vec3(0, s, 0)},  // top back
        {glm::vec3(s, 0, s),   glm::vec3(s, 0, -s), glm::vec3(0, s, 0)},  // top right
        {glm::vec3(-s, 0, -s), glm::vec3(s, 0, -s), glm::vec3(0, -s, 0)}, // bottom front
        {glm::vec3(-s, 0, -s), glm::vec3(-s, 0, s), glm::vec3(0, -s, 0)}, // bottom left
        {glm::vec3(s, 0, s),   glm::vec3(-s, 0, s), glm::vec3(0, -s, 0)}, // bottom back
        {glm::vec3(s, 0, s),   glm::vec3(s, 0, -s), glm::vec3(0, -s, 0)}, // bottom right
    };

    std::vector<Triangle> triangles;
    for (Triangle& triangle : startTriangles) {
        glm::vec3 start_v0 = triangle.points[0];
        glm::vec3 v0 = start_v0;
        glm::vec3 v = start_v0;
        glm::vec3 start_v1 = triangle.points[1];
        glm::vec3 start_v2 = triangle.points[2];
        for (int i = 0; i < num_samples + 1; i++) {
            glm::vec3 v1 = start_v0 + 1.0f / (num_samples + 1) * (i + 1) * (start_v1 - start_v0);
            glm::vec3 v2 = start_v0 + 1.0f / (num_samples + 1) * (i + 1) * (start_v2 - start_v0);;
            int numTri = i * 2 + 1;
            for (int j = 0; j < numTri; j++) {
                if (j % 2 == 0) {
                    glm::vec3 v3;
                    if (i == 0) {
                        v3 = start_v0;
                    } else {
                        v3 = (v0 + 1.0f / float(i) * float(j / 2) * (v - v0));
                    }
                    glm::vec3 v4 = (v1 + 1.0f / float(i + 1) * float(j / 2) * (v2 - v1));
                    glm::vec3 v5 = (v1 + 1.0f / float(i + 1) * float(j / 2 + 1) * (v2 - v1));
    
                    triangles.push_back({v3, v4, v5});
                } else {
                    glm::vec3 v3;
                    if (i == 0) {
                        v3 = start_v0;
                    } else {
                        v3 = (v0 + 1.0f / float(i) * float(j / 2) * (v - v0));
                    }
                    glm::vec3 v4 = (v1 + 1.0f / float(i + 1) * float(j / 2 + 1) * (v2 - v1));
                    glm::vec3 v5 = (v0 + 1.0f / float(i) * float(j / 2 + 1) * (v - v0));
    
                    triangles.push_back({v3, v4, v5});
                }
            }
            v0 = v1;
            v = v2;
        }
    }

    printf("num triangles: %i\n", triangles.size());

    for (Triangle& triangle : triangles) {            
        glm::vec3 v0 = glm::normalize(triangle.points[0]);
        glm::vec3 v1 = glm::normalize(triangle.points[1]);
        glm::vec3 v2 = glm::normalize(triangle.points[2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        glm::vec3 color = glm::abs(normal);

        for (int i = 0; i < 3; i++) {
            auto it = this->uniquePoints.find(triangle.points[i]);
            if (it == this->uniquePoints.end()) {
                this->uniquePoints.insert(triangle.points[i]);
                this->transformedPoints[triangle.points[i]] = this->transformPoint(triangle.points[i]);
            }
            glm::vec3 point = this->transformedPoints[triangle.points[i]];
            this->indices.push_back(this->vertices.size());
            this->vertices.push_back({point, color});
            //printf("%i\n", points[i]);
        }
    }

    printf("vertices: %i\n", this->vertices.size());
}

glm::vec3 Planet::transformPoint(glm::vec3 point) {
    return glm::normalize(point) * this->size * (static_cast<float>(rand()) / RAND_MAX / 2 + 1) + this->position;
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