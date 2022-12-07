#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../utils/sceneparser.h"
#include "shape.h"

class Cone: public Shape
{
public:
    Cone();
    ~Cone() override{};
    void updateParams(int param1, int param2) override;

private:
    void setVertexData();
    void makeBody();
    void makeTileCap(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeTileBody(glm::vec3 topLeft,
                  glm::vec3 topRight,
                  glm::vec3 bottomLeft,
                  glm::vec3 bottomRight);
    void makeCapBottom();
    glm::vec2 getUV(glm::vec3 pos, int faceIndex);

    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
