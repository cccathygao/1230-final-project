#include "cube.h"

Cube::Cube():
Shape(){

}

void Cube::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(1.f, static_cast<float>(param1));
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    int i,
                    int j) {
    // create a tile (i.e. 2 triangles) based on 4 given points.
    glm::vec3 normal = glm::normalize(glm::cross(bottomLeft - topLeft, topRight - topLeft));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(i, j+1));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(i, j));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(i+1, j));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(i+1, j));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(i+1, j+1));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(i, j+1));
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {

    glm::vec3 sidelen_h = (topRight - topLeft) / static_cast<float>(m_param1);
    glm::vec3 sidelen_v = (bottomLeft - topLeft) / static_cast<float>(m_param1);
    glm::vec3 rowStart = topLeft;
    for (int i = 0; i < m_param1; i = i+1){
        glm::vec3 colStart = rowStart;
        for (int j = 0; j < m_param1; j = j+1){
            makeTile(colStart, colStart + sidelen_h, colStart + sidelen_v, colStart + sidelen_h + sidelen_v, i, j);
            colStart += sidelen_h;
        }
        rowStart += sidelen_v;
    }
}

void Cube::setVertexData() {

     // z = 0.5
     makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f));

     // z = -0.5
     makeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f));

     // x = 0.5
     makeFace(glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f));

     // x = -0.5
     makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f));

     // y = 0.5
     makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f));

     // y = -0.5
     makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f));
}

glm::vec2 Cube::getUV(float i, float j){
    float u = j / m_param1;
    float v = 1 - i / m_param1;

    return glm::vec2(u, v);
}
