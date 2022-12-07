#ifndef SHAPE_H
#define SHAPE_H

#include "../utils/sceneparser.h"

class Shape {
public:
    Shape();
    virtual ~Shape() {};
    virtual void updateParams(int param1, int param2) = 0;
    std::vector<float> getVertexData() { return m_vertexData; }

protected:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void insertVec2(std::vector<float> &data, glm::vec2 v);

    std::vector<float> m_vertexData = {};
};

#endif // SHAPE_H
