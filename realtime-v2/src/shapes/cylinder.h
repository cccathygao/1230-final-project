#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "shape.h"

class Cylinder : public Shape
{
public:
    void updateParams(int param1, int param2);
    std::vector<float>& generateShape() { return m_vertexData; }
    void setWedge(glm::vec2 leftVertex, glm::vec2 rightVertex);
    void setBaseWedge(float angle1, float angle2);

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();

    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
