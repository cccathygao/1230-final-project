#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../utils/sceneparser.h"
#include "shape.h"
#include <cmath>

class Sphere: public Shape
{
public:
    Sphere();
    ~Sphere() override {};
    void updateParams(int param1, int param2) override;

private:
    void setVertexData();
    void makeTile(float currentTheta, float nextTheta, float currentPhi, float nextPhi);
    void makeWedge(float currTheta, float nextTheta);
    void makeSphere();
    glm::vec2 getUV(float theta, float phi);

    float m_radius = 0.5;
    int m_param1;
    int m_param2;
};
