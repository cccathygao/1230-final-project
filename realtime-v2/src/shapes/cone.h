#pragma once
#include "shape.h"
#include <vector>
#include <glm/glm.hpp>

class Cone : public Shape
{
public:
    void updateParams(int param1, int param2);
    std::vector<float>& generateShape() { return m_vertexData; }
    glm::vec3 normalFinder(float x, float y,float z,float r, float theta);
    void setBaseWedge(float angle1, float angle2);
private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
    void setWedge(float theta1, float theta2);
    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
