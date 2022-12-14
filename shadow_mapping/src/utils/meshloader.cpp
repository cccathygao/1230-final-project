#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>
#include "meshloader.h"

using namespace std;
using namespace tinyobj;

// Inserts a glm::vec3 into a vector of floats.
void Loader::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

// Inserts a glm::vec2 into a vector of floats.
void Loader::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}

void Loader::load(const std::string &path) {

    m_vertexData.clear();

    string warn, err;
    ObjReader reader;
    ObjReaderConfig config;

    bool success = reader.ParseFromFile(path, config);
    if (!success) {
        cerr << "load obj fail: " << reader.Error() << endl;
        return;
    }

    if (!reader.Warning().empty()) {
        cout << "load obj warning: " << reader.Warning() << endl;
    }

    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();
    const auto &materials = reader.GetMaterials();

    // loop over shapes
    for (const auto &shape : shapes) {
        size_t indexOffset = 0;

        cout << "number of faces: " << shape.mesh.num_face_vertices.size() << endl;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            size_t fv = shape.mesh.num_face_vertices[f];

            cout << "number of vertices for curr face: " << fv << endl;

            if (fv != 3) {
                cout << "not triangle" << endl;
            }

            // loop over vertices in the face
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                insertVec3(m_vertexData, glm::vec3(vx, vy, vz));

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    insertVec3(m_vertexData, glm::vec3(nx, ny, nz));
                } else {
                    // use normal (0, 1, 0) if obj does not have normal specified.
                    insertVec3(m_vertexData, glm::vec3(0, 1, 0));
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    insertVec2(m_vertexData, glm::vec2(tx, ty));
                } else {
                    // use uv coordinate (0, 0) if obj does not have uv specified.
                    insertVec2(m_vertexData, glm::vec2(0, 0));
                }
            }
            indexOffset += fv;

            // access to material of the face
            if (shape.mesh.material_ids[f] >= 0) {

            }
        }
    }
    return;
}
