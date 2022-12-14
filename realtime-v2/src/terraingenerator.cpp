#include "terraingenerator.h"

#include "glm/glm.hpp"
#include <cmath>
#include <iostream>

// Constructor
TerrainGenerator::TerrainGenerator() {
    // Task 8: turn off wireframe shading
    m_wireshade = false; // STENCIL CODE
    // m_wireshade = false; // TA SOLUTION

    // Define resolution of terrain generation
    m_resolution = 100;

    // Generate random vector lookup table
    m_lookupSize = 1024;
    m_randVecLookup.reserve(m_lookupSize);

    // Initialize random number generator
    std::srand(1230);

    // Populate random vector lookup table
    for (int i = 0; i < m_lookupSize; i++) {
        m_randVecLookup.push_back(glm::vec2(std::rand() * 2.0 / RAND_MAX - 1.0,
                                            std::rand() * 2.0 / RAND_MAX - 1.0));
    }
}

// Destructor
TerrainGenerator::~TerrainGenerator() { m_randVecLookup.clear(); }

// Helper for generateTerrain()
void addPointToVector(glm::vec3 point, std::vector<float> &vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}

// Generates the geometry of the output triangle mesh
std::vector<float> TerrainGenerator::generateTerrain() {
    std::vector<float> verts;
    verts.reserve(m_resolution * m_resolution * 6);

    for (int x = 0; x < m_resolution; x++) {
        for (int y = 0; y < m_resolution; y++) {
            int x1 = x;
            int y1 = y;

            int x2 = x + 1;
            int y2 = y + 1;

            glm::vec3 p1 = getPosition(x1, y1);
            glm::vec3 p2 = getPosition(x2, y1);
            glm::vec3 p3 = getPosition(x2, y2);
            glm::vec3 p4 = getPosition(x1, y2);

            glm::vec3 n1 = getNormal(x1, y1);
            glm::vec3 n2 = getNormal(x2, y1);
            glm::vec3 n3 = getNormal(x2, y2);
            glm::vec3 n4 = getNormal(x1, y2);

            // tris 1
            // x1y1z1
            // x2y1z2
            // x2y2z3
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p2, verts);
            addPointToVector(n2, verts);
            addPointToVector(getColor(n2, p2), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3, p3), verts);

            // tris 2
            // x1y1z1
            // x2y2z3
            // x1y2z4
            addPointToVector(p1, verts);
            addPointToVector(n1, verts);
            addPointToVector(getColor(n1, p1), verts);

            addPointToVector(p3, verts);
            addPointToVector(n3, verts);
            addPointToVector(getColor(n3, p3), verts);

            addPointToVector(p4, verts);
            addPointToVector(n4, verts);
            addPointToVector(getColor(n4, p4), verts);
        }
    }
    return verts;
}

// Samples the (infinite) random vector grid at (row, col)
glm::vec2 TerrainGenerator::sampleRandomVector(int row, int col) {
    std::hash<int> intHash;
    int index = intHash(row * 41 + col * 43) % m_lookupSize;
    return m_randVecLookup.at(index);
}

// Takes a grid coordinate (row, col), [0, m_resolution), which describes a
// vertex in a plane mesh Returns a normalized position (x, y, z); x and y in
// range from [0, 1), and z is obtained from getHeight()
glm::vec3 TerrainGenerator::getPosition(int row, int col) {
    // Normalizing the planar coordinates to a unit square
    // makes scaling independent of sampling resolution.
    float x = 1.0 * row / m_resolution;
    float y = 1.0 * col / m_resolution;
    float z = getHeight(x, y);
    return glm::vec3(x, y, z);
}

// ================== Students, please focus on the code below this point

// Helper for computePerlin() and, possibly, getColor()
float interpolate(float A, float B, float alpha) {
    // Task 4: implement your easing/interpolation function below
    float ease = (float)(3 * pow(alpha, 2) - 2 * pow(alpha, 3));
    return A + ease * (B - A);
}

// Takes a normalized (x, y) position, in range [0,1)
// Returns a height value, z, by sampling a noise function
float TerrainGenerator::getHeight(float x, float y) {

    // Task 6: modify this call to produce noise of a different frequency
    float z = computePerlin(x * 5, y * 5) / 2;

    // Task 7: combine multiple different octaves of noise to produce fractal
    // perlin noise
    float a = computePerlin(x * 2, y * 2) / 2;
    float b = computePerlin(x * 4, y * 4) / 2;
    float c = computePerlin(x * 8, y * 8) / 8;
    float d = computePerlin(x * 16, y * 16) / 16;

    // Return 0 as placeholder
    return z + a + b + c + d;
}

// Computes the normal of a vertex by averaging neighbors
glm::vec3 TerrainGenerator::getNormal(int row, int col) {
    // Task 9: Compute the average normal for the given input indices
    glm::vec3 normal = glm::vec3(0, 0, 0);
    std::vector<std::vector<int>> neighborOffsets = {
        // Counter-clockwise around the vertex
        {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    glm::vec3 V = getPosition(row, col);
    for (int i = 0; i < 8; ++i) {
        int n1RowOffset = neighborOffsets[i][0];
        int n1ColOffset = neighborOffsets[i][1];
        int n2RowOffset = neighborOffsets[(i + 1) % 8][0];
        int n2ColOffset = neighborOffsets[(i + 1) % 8][1];
        glm::vec3 n1 = getPosition(row + n1RowOffset, col + n1ColOffset);
        glm::vec3 n2 = getPosition(row + n2RowOffset, col + n2ColOffset);
        normal = normal + glm::cross(n1 - V, n2 - V);
    }
    return glm::normalize(normal);
}

// Computes color of vertex using normal and, optionally, position
glm::vec3 TerrainGenerator::getColor(glm::vec3 normal, glm::vec3 position) {
    // Task 10: compute color as a function of the normal and position
    if (glm::dot(normal, glm::vec3(1,1,0)) == 0.0) {
        return glm::vec3(1, 1, 1);
    }
    else if (position.z > 0) {
        return glm::vec3(1, 1, 1);
    } else {
        return glm::vec3(0.5, 0.5, 0.5);
    }
}

// Computes the intensity of Perlin noise at some point
float TerrainGenerator::computePerlin(float x, float y) {
    // Task 1: get grid indices (as ints)
    auto gridp1 = glm::vec2((int)x, (int)y);
    auto gridp2 = glm::vec2((int)x + 1, (int)y);
    auto gridp3 = glm::vec2((int)x, (int)y + 1);
    auto gridp4 = glm::vec2((int)x + 1, (int)y + 1);

    // Task 2: compute offset vectors
    glm::vec2 offset1 = glm::vec2(x, y) - gridp1;
    glm::vec2 offset2 = glm::vec2(x, y) - gridp2;
    glm::vec2 offset3 = glm::vec2(x, y) - gridp3;
    glm::vec2 offset4 = glm::vec2(x, y) - gridp4;

    // Task 3: compute the dot product between offset and grid vectors
    float A = glm::dot(offset1, sampleRandomVector(gridp1.x, gridp1.y));
    float B = glm::dot(offset2, sampleRandomVector(gridp2.x, gridp2.y));
    float C = glm::dot(offset3, sampleRandomVector(gridp3.x, gridp3.y));
    float D = glm::dot(offset4, sampleRandomVector(gridp4.x, gridp4.y));

    // Task 5: use your interpolation function to produce the final value
    float G = interpolate(A, B, x - gridp1.x);
    float H = interpolate(C, D, x - gridp1.x);
    float R = interpolate(G, H, y - gridp1.y);

    return R;
}
