#include "cone.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

Cone::Cone():
Shape(){

}

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(1.f, static_cast<float>(param1));
    m_param2 = std::max(3.f, static_cast<float>(param2));
    setVertexData();
}

void Cone::makeTileCap(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 normal = glm::normalize(glm::cross(bottomLeft - topLeft, topRight - topLeft));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(topRight, 0));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(topLeft, 0));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(bottomLeft, 0));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(bottomLeft, 0));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(bottomRight, 0));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);
    insertVec2(m_vertexData, getUV(topRight, 0));
}

void Cone::makeTileBody(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {

    float TRnormal_y = glm::length(glm::vec3(topRight.x, 0.f, topRight.z)) / 2.0;
    glm::vec3 TRnormal = glm::normalize(glm::vec3(0.0, TRnormal_y, 0.0) + glm::vec3(topRight.x, 0.f, topRight.z));

    float TLnormal_y = glm::length(glm::vec3(topLeft.x, 0.f, topLeft.z)) / 2.0;
    glm::vec3 TLnormal = glm::normalize(glm::vec3(0.0, TLnormal_y, 0.0) + glm::vec3(topLeft.x, 0.f, topLeft.z));

    float BLnormal_y = glm::length(glm::vec3(bottomLeft.x, 0.f, bottomLeft.z)) / 2.0;
    glm::vec3 BLnormal = glm::normalize(glm::vec3(0.0, BLnormal_y, 0.0) + glm::vec3(bottomLeft.x, 0.f, bottomLeft.z));

    float BRnormal_y = glm::length(glm::vec3(bottomRight.x, 0.f, bottomRight.z)) / 2.0;
    glm::vec3 BRnormal = glm::normalize(glm::vec3(0.0, BRnormal_y, 0.0) + glm::vec3(bottomRight.x, 0.f, bottomRight.z));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, TRnormal);
    insertVec2(m_vertexData, getUV(topRight, 1));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, TLnormal);
    insertVec2(m_vertexData, getUV(topLeft, 1));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, BLnormal);
    insertVec2(m_vertexData, getUV(bottomLeft, 1));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, BLnormal);
    insertVec2(m_vertexData, getUV(bottomLeft, 1));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, BRnormal);
    insertVec2(m_vertexData, getUV(bottomRight, 1));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, TRnormal);
    insertVec2(m_vertexData, getUV(topRight, 1));
}

void Cone::makeBody() {
    float step_v = 1.f / m_param1; // vertical step
    float step_h = 0.5 / m_param1; // horizontal step
    float angle = glm::radians(360.f / m_param2);
    glm::vec3 center = glm::vec3(0.f, 0.5, 0.f);

    for (int i = 0; i < m_param2; i += 1){
        float currentTheta = angle * i;
        float nextTheta = angle * (i+1);

        float x01 = step_h * glm::cos(currentTheta);
        float z01 = step_h * glm::sin(currentTheta);
        glm::vec3 p01 = glm::vec3(x01, 0.5-step_v, z01);

        float x02 = step_h * glm::cos(nextTheta);
        float z02 = step_h * glm::sin(nextTheta);
        glm::vec3 p02 = glm::vec3(x02, 0.5-step_v, z02);

        float normal1_y = glm::length(glm::vec3(x01, 0.f, z01)) / 2.0;
        glm::vec3 normal1 = glm::normalize(glm::vec3(0.0, normal1_y, 0.0) + glm::vec3(x01, 0.f, z01));

        float normal2_y = glm::length(glm::vec3(x02, 0.f, z02)) / 2.0;
        glm::vec3 normal2 = glm::normalize(glm::vec3(0.0, normal2_y, 0.0) + glm::vec3(x02, 0.f, z02));

        // center for normal
        float center_x = step_h * glm::cos(angle * (i+0.5));
        float center_z = step_h * glm::sin(angle * (i+0.5));

        float normal_center_y = glm::length(glm::vec3(center_x, 0.f, center_z)) / 2.0;
        glm::vec3 normal_center = glm::normalize(glm::vec3(0.0, normal_center_y, 0.0) + glm::vec3(center_x, 0.f, center_z));

        insertVec3(m_vertexData, p01);
        insertVec3(m_vertexData, normal1);
        insertVec2(m_vertexData, getUV(p01, 1));

        insertVec3(m_vertexData, center);
        insertVec3(m_vertexData, normal_center);
        insertVec2(m_vertexData, getUV(center, 1));

        insertVec3(m_vertexData, p02);
        insertVec3(m_vertexData, normal2);
        insertVec2(m_vertexData, getUV(p02, 1));


        for (int j = 1; j < m_param1; j += 1){
            float x1 = step_h * j * glm::cos(currentTheta);
            float z1 = step_h * j * glm::sin(currentTheta);
            float x2 = step_h * j * glm::cos(nextTheta);
            float z2 = step_h * j * glm::sin(nextTheta);
            float x3 = step_h * (j+1) * glm::cos(currentTheta);
            float z3 = step_h * (j+1) * glm::sin(currentTheta);
            float x4 = step_h * (j+1) * glm::cos(nextTheta);
            float z4 = step_h * (j+1) * glm::sin(nextTheta);

            float y1 = 0.5 - step_v * j;
            glm::vec3 topLeft = glm::vec3(x1, y1, z1);
            glm::vec3 topRight = glm::vec3(x2, y1, z2);

            float y2 = 0.5 - step_v * (j+1);
            glm::vec3 bottomLeft = glm::vec3(x3, y2, z3);
            glm::vec3 bottomRight = glm::vec3(x4, y2, z4);

            makeTileBody(topRight, topLeft, bottomRight, bottomLeft);
        }
    }
}

void Cone::makeCapBottom(){
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
        insertVec2(m_vertexData, getUV(p01, 0));

        insertVec3(m_vertexData, p02);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(p02, 0));

        insertVec3(m_vertexData, center);
        insertVec3(m_vertexData, normal);
        insertVec2(m_vertexData, getUV(center, 0));


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

            makeTileCap(p1, p2, p3, p4);
        }
    }
}

void Cone::setVertexData() {
    makeBody();
    makeCapBottom();
}

glm::vec2 Cone::getUV(glm::vec3 pos, int faceIndex){
    float u, v;

    // bottom
    if (faceIndex == 0){
        u = pos.x + 0.5;
        v = pos.z + 0.5;
    } else{
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
