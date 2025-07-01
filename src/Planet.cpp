#include "Planet.hpp"

Planet::Planet(glm::vec3 pos, float size) {
    this->position = pos;
    this->size = size;
    this->icosahedron();
}

Planet::~Planet() {

}

void Planet::icosahedron() {
    std::vector<Triangle> startTriangles = {
        {glm::vec3(-1, 0, -1), glm::vec3(1, 0, -1), glm::vec3(0, 1, 0)},  // top front
        {glm::vec3(-1, 0, -1), glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0)},  // top left
        {glm::vec3(1, 0, 1),   glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0)},  // top back
        {glm::vec3(1, 0, 1),   glm::vec3(1, 0, -1), glm::vec3(0, 1, 0)},  // top right
        {glm::vec3(-1, 0, -1), glm::vec3(1, 0, -1), glm::vec3(0, -1, 0)}, // bottom front
        {glm::vec3(-1, 0, -1), glm::vec3(-1, 0, 1), glm::vec3(0, -1, 0)}, // bottom left
        {glm::vec3(1, 0, 1),   glm::vec3(-1, 0, 1), glm::vec3(0, -1, 0)}, // bottom back
        {glm::vec3(1, 0, 1),   glm::vec3(1, 0, -1), glm::vec3(0, -1, 0)}, // bottom right
    };

    std::vector<Triangle> triangles;
    int num = 10;
    for (Triangle& triangle : startTriangles) {
        glm::vec3 start_v0 = triangle.points[0];
        glm::vec3 v0 = start_v0;
        glm::vec3 v = start_v0;
        glm::vec3 start_v1 = triangle.points[1];
        glm::vec3 start_v2 = triangle.points[2];
        for (int i = 0; i < num + 1; i++) {
            glm::vec3 v1 = start_v0 + 1.0f / (num + 1) * (i + 1) * (start_v1 - start_v0);
            glm::vec3 v2 = start_v0 + 1.0f / (num + 1) * (i + 1) * (start_v2 - start_v0);;
            int numTri = i * 2 + 1;
            for (int j = 0; j < numTri; j++) {
                if (j % 2 == 0) {
                    glm::vec3 v3;
                    if (i == 0) {
                        v3 = start_v0;
                    } else {
                        v3 = glm::normalize(v0 + 1.0f / float(i) * float(j / 2) * (v - v0));
                    }
                    glm::vec3 v4 = glm::normalize(v1 + 1.0f / float(i + 1) * float(j / 2) * (v2 - v1));
                    glm::vec3 v5 = glm::normalize(v1 + 1.0f / float(i + 1) * float(j / 2 + 1) * (v2 - v1));
    
                    triangles.push_back({v3, v4, v5});
                } else {
                    glm::vec3 v3;
                    if (i == 0) {
                        v3 = start_v0;
                    } else {
                        v3 = glm::normalize(v0 + 1.0f / float(i) * float(j / 2) * (v - v0));
                    }
                    glm::vec3 v4 = glm::normalize(v1 + 1.0f / float(i + 1) * float(j / 2 + 1) * (v2 - v1));
                    glm::vec3 v5 = glm::normalize(v0 + 1.0f / float(i) * float(j / 2 + 1) * (v - v0));
    
                    triangles.push_back({v3, v4, v5});
                }
            }
            v0 = v1;
            v = v2;
        }
    }

    /* for (int i = 0; i < 5; i++) {
        std::vector<Triangle> outputTriangles;
        for (Triangle& triangle : triangles) {
            glm::vec3 v0 = triangle.points[0];
            glm::vec3 v1 = triangle.points[1];
            glm::vec3 v2 = triangle.points[2];
            glm::vec3 v3 = glm::normalize(0.5f * (v0 + v1));
            glm::vec3 v4 = glm::normalize(0.5f * (v1 + v2));
            glm::vec3 v5 = glm::normalize(0.5f * (v2 + v0));
            outputTriangles.push_back({v0, v3, v5});
            outputTriangles.push_back({v3, v1, v4});
            outputTriangles.push_back({v4, v2, v5});
            outputTriangles.push_back({v3, v4, v5});
        }
        triangles = outputTriangles;
    } */

    for (Triangle& triangle : triangles) {
        for (int i = 0; i < 3; i++) {
            triangle.points[i] = glm::normalize(triangle.points[i]) * this->size;
        }
        glm::vec3 normal = glm::normalize(glm::cross(triangle.points[1] - triangle.points[0], triangle.points[2] - triangle.points[0]));
        glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.0f) + normal * 0.5f;
        for (glm::vec3 point : triangle.points) {
            this->indices.push_back(this->vertices.size());
            this->vertices.push_back({{point.x * this->size + this->position.x, point.y * this->size + this->position.y, point.z * this->size + this->position.z}, {color.x, color.y, color.z}});
        }
    }
}

void Planet::fibonacciSphere() {
    float phi = M_PI * (sqrt(5.0f) - 1.0f);  // golden angle in radians

    for (int i = 0; i < samples; i++) {
        float y = 1 - (i / float(samples - 1)) * 2;  // y goes from 1 to -1
        float radius = sqrt(1 - y * y); //radius at y

        float theta = phi * i;  // golden angle increment

        float x = cos(theta) * radius;
        float z = sin(theta) * radius;

        glm::vec3 newPoint = glm::vec3(x, y, z);


        this->points.push_back(newPoint);
    }

    /* auto rng = std::default_random_engine {};
    std::shuffle(std::begin(this->points), std::end(this->points), rng); */
    //std::reverse(this->points.begin(), this->points.end());
}

void Planet::bowyer_watson() {
    std::vector<Triangle> triangulation;
    //printf("%i\n", glm::vec3(-this->size * 2, 0, this->size * 2).length());
    int n = M_PI * 2;
    Triangle superTriangle = Triangle(glm::vec3(-n, 0, n), glm::vec3(0, 0, -n), glm::vec3(n, 0, n));
    triangulation.push_back(superTriangle);

    int count = 0;

    for (glm::vec3 point : this->points) {
        //printf("Point: %f, %f, %f\n", point.x, point.y, point.z);
        std::vector<Triangle> badTriangles = {};
        for (Triangle triangle : triangulation) {
            if (triangle.pointIsInside(point)) {
                //printf("aaaa\n");
                badTriangles.push_back(triangle);
            }
        }
        //printf("%i\n", badTriangles.size());
        std::vector<Edge> polygon = {};
        for (Triangle triangle : badTriangles) {
            for (int i = 0; i < 3; i++) {
                if (!isEdgeShared(triangle.edges[i], triangle, badTriangles)) {
                    polygon.push_back(triangle.edges[i]);
                }
            }
        }
        for (const Triangle& triangle : badTriangles) {
            triangulation.erase(std::remove(triangulation.begin(), triangulation.end(), triangle), triangulation.end());
        }
        for (Edge edge : polygon) {
            Triangle newTri(edge.points[0], edge.points[1], point);
            // Compute normal
            /* glm::vec3 normal = glm::normalize(glm::cross(
                newTri.points[1] - newTri.points[0],
                newTri.points[2] - newTri.points[0]
            ));
            glm::vec3 centroid = (newTri.points[0] + newTri.points[1] + newTri.points[2]) / 3.0f;
            // If normal points inward, swap two vertices to flip winding
            if (glm::dot(normal, centroid) > 0.0f) {
                std::swap(newTri.points[1], newTri.points[2]);
                // Also update edges if needed
                newTri.edges[0] = Edge(newTri.points[0], newTri.points[1]);
                newTri.edges[1] = Edge(newTri.points[1], newTri.points[2]);
                newTri.edges[2] = Edge(newTri.points[2], newTri.points[0]);
            } */
            triangulation.push_back(newTri);
            count++;
        }
    }

    printf("Triangles: %i\n", triangulation.size());
    printf("count: %i\n", count);
    
    auto isSuperTriangleVertex = [&](const glm::vec3& v) {
        return v == superTriangle.points[0] ||
            v == superTriangle.points[1] ||
            v == superTriangle.points[2];
    };

    triangulation.erase(
        std::remove_if(triangulation.begin(), triangulation.end(),
            [&](const Triangle& tri) {
                return isSuperTriangleVertex(tri.points[0]) ||
                    isSuperTriangleVertex(tri.points[1]) ||
                    isSuperTriangleVertex(tri.points[2]);
            }),
        triangulation.end()
    );

    auto isDegenerate = [](const Triangle& tri) {
        return tri.points[0] == tri.points[1] ||
            tri.points[1] == tri.points[2] ||
            tri.points[2] == tri.points[0];
    };

    for (const Triangle& triangle : triangulation) {
        if (isDegenerate(triangle)) continue;
        glm::vec3 normal = glm::normalize(glm::cross(triangle.points[1] - triangle.points[0], triangle.points[2] - triangle.points[0]));
        glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.0f) + normal * 0.5f;
        for (glm::vec3 point : triangle.points) {
            this->indices.push_back(this->vertices.size());
            this->vertices.push_back({{point.x * this->size + this->position.x, point.y * this->size + this->position.y, point.z * this->size + this->position.z}, {color.x, color.y, color.z}});
        }
    }
}

Triangle::Triangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
    this->points[0] = point1;
    this->points[1] = point2;
    this->points[2] = point3;
    this->center = glm::vec3((point1.x + point2.x + point3.x) / 3, (point1.y + point2.y + point3.y) / 3, (point1.z + point2.z + point3.z) / 3);
    //printf("%f, %f, %f, %f\n", glm::length(this->center), glm::length(point1), glm::length(point2), glm::length(point3));
    //printf("Size: %f\n", this->size);
    this->edges[0] = Edge(point1, point2);
    this->edges[1] = Edge(point2, point3);
    this->edges[2] = Edge(point3, point1);
}

bool Triangle::pointIsInside(glm::vec3 point) {
    /* printf("Point: %f, %f, %f\n", point.x, point.y, point.z);
    printf("Center: %f, %f, %f\n", this->center.x, this->center.y, this->center.z);
    printf("Size: %f\n", this->size);
    printf("Distance: %f\n", glm::distance(point, this->center)); */
    // Compute the circumcenter and radius
    glm::vec3 a = points[0];
    glm::vec3 b = points[1];
    glm::vec3 c = points[2];

    /* glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 abXac = glm::cross(ab, ac);

    glm::vec3 toCircumsphereCenter = (glm::cross( abXac, ab ) * glm::length(ac) * glm::length(ac) + glm::cross( ac, abXac ) * glm::length(ab) * glm::length(ab)) / (2.f * glm::length(abXac) * glm::length(abXac)) ;

    float circumsphereRadius = glm::length(toCircumsphereCenter);

    glm::vec3 ccs = a + toCircumsphereCenter;

    return glm::length(point - ccs) <= circumsphereRadius; */

    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 abXac = glm::cross(ab, ac);

    float denominator = 2.0f * glm::dot(abXac, abXac);
    if (denominator == 0.0f) return false; // Degenerate triangle
    
    glm::vec3 normal = glm::normalize(abXac);
    // Check if the point is in the same hemisphere as the triangle
    /* if (glm::dot(normal, point - a) < 0.0f) {
        // Point is not in the same hemisphere as the triangle
        return false;
    } */

   
   glm::vec3 toCircumcenter =
        (glm::cross(abXac, ab) * glm::dot(ac, ac) +
         glm::cross(ac, abXac) * glm::dot(ab, ab)) / denominator;
         
    glm::vec3 circumcenter = a + toCircumcenter;
    
    float radius = glm::length(toCircumcenter);

    /* if (sign(center.y) != sign(point.y)) {
        printf("{(%f, %f, %f), (%f, %f, %f), %f}; ", circumcenter.x, circumcenter.y, circumcenter.z, point.x, point.y, point.z, radius);
    } */
    
   if (sign(point.y) != sign(abXac.y)) {
       // Point is not in the same hemisphere as the triangle
       //printf("{(%f, %f, %f), (%f, %f, %f), %f}; ", circumcenter.x, circumcenter.y, circumcenter.z, point.x, point.y, point.z, radius);
       return false;
   }
   bool isInsideCircumcircle = glm::length(point - circumcenter) <= radius + 0.0001f; // Adding a small epsilon to account for floating point precision issues
    /* if (sign(center.y) != sign(point.y)) {
        //printf("(%i, %i), ", sign(point.y), isInsideCircumcircle); // Point is not in the same hemisphere as the triangle
        return false; // Point is not in the same hemisphere as the triangle
    } */
    return isInsideCircumcircle;
}

Edge::Edge(glm::vec3 point1, glm::vec3 point2) {
    this->points[0] = point1;
    this->points[1] = point2;
}

Edge::Edge() {
    this->points[0] = glm::vec3(0, 0, 0);
    this->points[1] = glm::vec3(0, 0, 0);
}

bool areEdgesEqual(const Edge& edge1, const Edge& edge2) {
    return (edge1.points[0] == edge2.points[0] && edge1.points[1] == edge2.points[1]) ||
           (edge1.points[0] == edge2.points[1] && edge1.points[1] == edge2.points[0]);
}

bool operator==(const Triangle& triangle1, const Triangle& triangle2) {
    return (triangle1.points[0] == triangle2.points[0] && triangle1.points[1] == triangle2.points[1] && triangle1.points[2] == triangle2.points[2]);
}

bool isEdgeShared(Edge edge, const Triangle& current, std::vector<Triangle> triangles) {
    for (Triangle triangle : triangles) {
        if (triangle == current) {
            continue; // Skip the current triangle
        }
        for (int i = 0; i < 3; i++) {
            if (areEdgesEqual(edge, triangle.edges[i])) {
                return true;
            }
        }
    }
    return false;
}

int sign(float value) {
    return (value > 0) - (value < 0);
}