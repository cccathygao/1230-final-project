#include "sphere.h"

Sphere::Sphere():
Shape(){

}

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(2.f, static_cast<float>(param1));
    m_param2 = std::max(3.f, static_cast<float>(param2));
    setVertexData();
}

void Sphere::makeTile(float currentTheta, float nextTheta, float currentPhi, float nextPhi){
    float x1 = 0.5 * glm::sin(currentPhi) * glm::sin(currentTheta);
    float y1 = 0.5 * glm::cos(currentPhi);
    float z1 = 0.5 * glm::sin(currentPhi) * glm::cos(currentTheta);
    glm::vec3 topLeft = glm::vec3(x1, y1, z1);

    float x2 = 0.5 * glm::sin(currentPhi) * glm::sin(nextTheta);
    float y2 = 0.5 * glm::cos(currentPhi);
    float z2 = 0.5 * glm::sin(currentPhi) * glm::cos(nextTheta);
    glm::vec3 topRight = glm::vec3(x2, y2, z2);

    float x3 = 0.5 * glm::sin(nextPhi) * glm::sin(currentTheta);
    float y3 = 0.5 * glm::cos(nextPhi);
    float z3 = 0.5 * glm::sin(nextPhi) * glm::cos(currentTheta);
    glm::vec3 bottomLeft = glm::vec3(x3, y3, z3);

    float x4 = 0.5 * glm::sin(nextPhi) * glm::sin(nextTheta);
    float y4 = 0.5 * glm::cos(nextPhi);
    float z4 = 0.5 * glm::sin(nextPhi) * glm::cos(nextTheta);
    glm::vec3 bottomRight = glm::vec3(x4, y4, z4);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRight - glm::vec3(0.f));
    insertVec2(m_vertexData, getUV(nextTheta, currentPhi));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeft - glm::vec3(0.f));
    insertVec2(m_vertexData, getUV(currentTheta, currentPhi));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeft - glm::vec3(0.f));
    insertVec2(m_vertexData, getUV(currentTheta, nextPhi));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeft - glm::vec3(0.f));
    insertVec2(m_vertexData, getUV(currentTheta, nextPhi));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRight - glm::vec3(0.f));
    insertVec2(m_vertexData, getUV(nextTheta, nextPhi));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRight - glm::vec3(0.f));
    insertVec2(m_vertexData, getUV(nextTheta, currentPhi));
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    std::vector<glm::vec3> vertices;
    float angle = glm::radians(180.f / m_param1);
    for (int i = 0; i <= m_param1; i += 1){
        float currentPhi = angle * i;
        float nextPhi = angle * (i+1);
        makeTile(currentTheta, nextTheta, currentPhi, nextPhi);
    }
}

void Sphere::makeSphere() {
    float angle = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i += 1){
        float currentTheta = angle * i;
        float nextTheta = angle * (i+1);
        makeWedge(currentTheta, nextTheta);
    }
}

void Sphere::setVertexData() {

     makeSphere();
}


glm::vec2 Sphere::getUV(float theta, float phi){
    float u = theta / (2 * M_PI);
    float v = 1 - phi / M_PI;

    return glm::vec2(u, v);
}
