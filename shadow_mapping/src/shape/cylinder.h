#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../utils/sceneparser.h"
#include "shape.h"

class Cylinder: public Shape
{
public:
    Cylinder();
    ~Cylinder() override {};
    void updateParams(int param1, int param2) override;

private:
    void setVertexData();
    void makeTileCap(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  int faceIndex);
    void makeTileBody(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeBody();
    void makeCapTop();
    void makeCapBottom();
    glm::vec2 getUV(glm::vec3 pos, int faceIndex);

    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
