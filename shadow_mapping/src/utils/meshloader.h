#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <vector>
#include <glm/glm.hpp>
#include "utils/tiny_obj_loader.h"
#include <iostream>
#include <filesystem>

class Loader {
public:
    Loader();
    virtual ~Loader() {};
    void load(const std::string &path);
    std::vector<float> getVertexData() { return m_vertexData; }

protected:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void insertVec2(std::vector<float> &data, glm::vec2 v);

    std::vector<float> m_vertexData = {};
};

#endif // MESHLOADER_H
