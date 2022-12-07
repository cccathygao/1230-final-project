#include "cylinder.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

Cylinder::Cylinder():
Shape(){

}

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(1.f, static_cast<float>(param1));
    m_param2 = std::max(3.f, static_cast<float>(param2));
    setVertexData();
}

void Cylinder::makeTileCap(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    int faceIndex) {
    glm::vec3 normal = glm::normalize(glm::cross(bottomLeft - topLeft, topRight - topLeft));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(topRight, faceIndex));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(topLeft, faceIndex));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(bottomLeft, faceIndex));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(bottomLeft, faceIndex));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(bottomRight, faceIndex));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(topRight, faceIndex));
}

void Cylinder::makeTileBody(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 TRnormal = glm::vec3(topRight.x, 0.f, topRight.z);
    glm::vec3 TLnormal = glm::vec3(topLeft.x, 0.f, topLeft.z);
    glm::vec3 BLnormal = glm::vec3(bottomLeft.x, 0.f, bottomLeft.z);
    glm::vec3 BRnormal = glm::vec3(bottomRight.x, 0.f, bottomRight.z);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, TRnormal);
    insertVec2(m_vertexData, getUV(topRight, 2));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, TLnormal);
    insertVec2(m_vertexData, getUV(topLeft, 2));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, BLnormal);
    insertVec2(m_vertexData, getUV(bottomLeft, 2));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, BLnormal);
    insertVec2(m_vertexData, getUV(bottomLeft, 2));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, BRnormal);
    insertVec2(m_vertexData, getUV(bottomRight, 2));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, TRnormal);
    insertVec2(m_vertexData, getUV(topRight, 2));
}

void Cylinder::makeBody() {
    float step = 1.f / m_param1; // vertical step
    float angle = glm::radians(360.f / m_param2);

    for (int i = 0; i < m_param2; i += 1){
        float currentTheta = angle * i;
        float nextTheta = angle * (i+1);

        float x1 = 0.5 * glm::cos(currentTheta);
        float z1 = 0.5 * glm::sin(currentTheta);
        float x2 = 0.5 * glm::cos(nextTheta);
        float z2 = 0.5 * glm::sin(nextTheta);

        for (int j = 0; j < m_param1; j += 1){

            float y1 = 0.5 - step * j;
            glm::vec3 topLeft = glm::vec3(x1, y1, z1);
            glm::vec3 topRight = glm::vec3(x2, y1, z2);

            float y2 = 0.5 - step * (j+1);
            glm::vec3 bottomLeft = glm::vec3(x1, y2, z1);
            glm::vec3 bottomRight = glm::vec3(x2, y2, z2);

            makeTileBody(topRight, topLeft, bottomRight, bottomLeft);
        }
    }
}

void Cylinder::makeCapTop(){
    float step = 0.5 / m_param1; // horizontal step
    float angle = glm::radians(360.f / m_param2);
    glm::vec3 center = glm::vec3(0.f, 0.5, 0.f);
    float y = 0.5;

    for (int i = 0; i < m_param2; i += 1){
        float currentTheta = angle * i;
        float nextTheta = angle * (i+1);

        float x01 = step * glm::cos(currentTheta);
        float z01 = step * glm::sin(currentTheta);
        glm::vec3 p01 = glm::vec3(x01, y, z01);

        float x02 = step * glm::cos(nextTheta);
        float z02 = step * glm::sin(nextTheta);
        glm::vec3 p02 = glm::vec3(x02, y, z02);

        glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);

        insertVec3(m_vertexData, p01);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(p01, 0));

        insertVec3(m_vertexData, center);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(center, 0));

        insertVec3(m_vertexData, p02);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(p02, 0));


        for (int j = 1; j < m_param1; j += 1){
            float x1 = step * j * glm::cos(currentTheta);
            float z1 = step * j * glm::sin(currentTheta);
            float x2 = step * j * glm::cos(nextTheta);
            float z2 = step * j * glm::sin(nextTheta);
            float x3 = step * (j+1) * glm::cos(currentTheta);
            float z3 = step * (j+1) * glm::sin(currentTheta);
            float x4 = step * (j+1) * glm::cos(nextTheta);
            float z4 = step * (j+1) * glm::sin(nextTheta);

            glm::vec3 p1 = glm::vec3(x1, y, z1);
            glm::vec3 p2 = glm::vec3(x2, y, z2);
            glm::vec3 p3 = glm::vec3(x3, y, z3);
            glm::vec3 p4 = glm::vec3(x4, y, z4);

            makeTileCap(p2, p1, p4, p3, 0);
        }
    }
}

void Cylinder::makeCapBottom(){
    float step = 0.5 / m_param1;
    float angle = glm::radians(360.f / m_param2);
    glm::vec3 center = glm::vec3(0.f, -0.5, 0.f);
    float y = -0.5;

    for (int i = 0; i < m_param2; i += 1){
        float currentTheta = angle * i;
        float nextTheta = angle * (i+1);

        float x01 = step * glm::cos(currentTheta);
        float z01 = step * glm::sin(currentTheta);
        glm::vec3 p01 = glm::vec3(x01, y, z01);

        float x02 = step * glm::cos(nextTheta);
        float z02 = step * glm::sin(nextTheta);
        glm::vec3 p02 = glm::vec3(x02, y, z02);

        glm::vec3 normal = glm::vec3(0.0, -1.0, 0.0);

        insertVec3(m_vertexData, p01);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(p01, 1));

        insertVec3(m_vertexData, p02);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(p02, 1));

        insertVec3(m_vertexData, center);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(center, 1));

        for (int j = 1; j < m_param1; j += 1){
            float x1 = step * j * glm::cos(currentTheta);
            float z1 = step * j * glm::sin(currentTheta);
            float x2 = step * j * glm::cos(nextTheta);
            float z2 = step * j * glm::sin(nextTheta);
            float x3 = step * (j+1) * glm::cos(currentTheta);
            float z3 = step * (j+1) * glm::sin(currentTheta);
            float x4 = step * (j+1) * glm::cos(nextTheta);
            float z4 = step * (j+1) * glm::sin(nextTheta);

            glm::vec3 p1 = glm::vec3(x1, y, z1);
            glm::vec3 p2 = glm::vec3(x2, y, z2);
            glm::vec3 p3 = glm::vec3(x3, y, z3);
            glm::vec3 p4 = glm::vec3(x4, y, z4);

            makeTileCap(p1, p2, p3, p4, 1);
        }
    }
}

void Cylinder::setVertexData() {

    makeBody();
    makeCapTop();
    makeCapBottom();
}

glm::vec2 Cylinder::getUV(glm::vec3 pos, int faceIndex){
    float u, v;

    // top
    if (faceIndex == 0){
        u = pos.x + 0.5;
        v = -pos.z + 0.5;
    }

    // bottom
    else if (faceIndex == 1){
        u = pos.x + 0.5;
        v = pos.z + 0.5;
    }

    else{
        // v = y + 0.5
        v = pos.y + 0.5;

        float theta = std::atan2(pos.z, pos.x);
        if (theta < 0) {
            u = -theta / (2 * M_PI);
        } else {
            u = 1.f - theta / (2 * M_PI);
        }
    }

    return glm::vec2(u, v);
}
