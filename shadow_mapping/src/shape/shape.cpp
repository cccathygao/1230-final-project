#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "shape.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>

Shape::Shape(){

}

// Inserts a glm::vec3 into a vector of floats.
void Shape::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

// Inserts a glm::vec2 into a vector of floats.
void Shape::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}
