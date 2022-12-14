#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../utils/sceneparser.h"
#include "shape.h"

class Cube: public Shape
{
public:
    Cube();
    ~Cube() override {};
    void updateParams(int param1, int param2) override;

private:
    void setVertexData();
    void makeTile(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight,
                  int i, int j);
    void makeFace(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    glm::vec2 getUV(float i, float j);

    int m_param1;
};
