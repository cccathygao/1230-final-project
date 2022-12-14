#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <vector>
class Shape
{
public:
    virtual void updateParams(int param1, int param2) = 0;
    virtual std::vector<float>& generateShape() = 0;
private:
    std::vector<float> m_vertexData;
};
