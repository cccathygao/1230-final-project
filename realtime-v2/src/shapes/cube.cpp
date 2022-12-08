
#include "cube.h"
#include <iostream>

void Cube::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(param1,1);
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
    glm::vec3 normal = glm::vec3(0.f,0.f,1.f);
    insertVec3(m_vertexData,topLeft);
    insertVec3(m_vertexData,glm::normalize(glm::cross(bottomLeft-topLeft,bottomRight-topLeft)));
    insertVec3(m_vertexData,bottomLeft);
    insertVec3(m_vertexData,glm::normalize(glm::cross(bottomRight-bottomLeft,topLeft-bottomLeft)));
    insertVec3(m_vertexData,bottomRight);
    insertVec3(m_vertexData,glm::normalize(glm::cross(topLeft-bottomRight,bottomLeft-bottomRight)));

    insertVec3(m_vertexData,topLeft);
    insertVec3(m_vertexData,glm::normalize(glm::cross(bottomRight-topLeft,topRight-topLeft)));
    insertVec3(m_vertexData,bottomRight);
    insertVec3(m_vertexData,glm::normalize(glm::cross(topRight-bottomRight,topLeft-bottomRight)));
    insertVec3(m_vertexData,topRight);
    insertVec3(m_vertexData,glm::normalize(glm::cross(topLeft-topRight,bottomRight-topRight)));
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube
    glm::vec3 normal = glm::normalize(glm::cross(bottomLeft-topLeft,bottomRight-topLeft));

    if(normal[2] == 1.f){
        float length = std::abs(topLeft[0]-topRight[0])/(m_param1+0.f);
        for(int i = 0; i<m_param1; i++){
            for(int j = 0; j<m_param1;j++){
                glm::vec3 topl = glm::vec3(bottomLeft[0]+j*length,bottomLeft[1]+(1+i)*length,bottomLeft[2]);
                glm::vec3 topr = glm::vec3(bottomLeft[0]+(1+j)*length,bottomLeft[1]+(1+i)*length,bottomLeft[2]);
                glm::vec3 botl = glm::vec3(bottomLeft[0]+j*length,bottomLeft[1]+i*length,bottomLeft[2]);
                glm::vec3 botr = glm::vec3(bottomLeft[0]+(1+j)*length,bottomLeft[1]+i*length,bottomLeft[2]);
                makeTile(topl,topr,botl,botr);
            }
        }
    }
    else if(normal[2] == -1.f){
        float length = std::abs(topLeft[0]-topRight[0])/(m_param1+0.f);
        for(int i = 0; i<m_param1; i++){
            for(int j = 0; j<m_param1;j++){
                glm::vec3 topl = glm::vec3(bottomLeft[0]-j*length,bottomLeft[1]+(1+i)*length,bottomLeft[2]);
                glm::vec3 topr = glm::vec3(bottomLeft[0]-(1+j)*length,bottomLeft[1]+(1+i)*length,bottomLeft[2]);
                glm::vec3 botl = glm::vec3(bottomLeft[0]-j*length,bottomLeft[1]+i*length,bottomLeft[2]);
                glm::vec3 botr = glm::vec3(bottomLeft[0]-(1+j)*length,bottomLeft[1]+i*length,bottomLeft[2]);
                makeTile(topl,topr,botl,botr);
            }
        }
    }
    // ----------
    if(normal[0] == 1.f){
        float length = std::abs(topLeft[2]-topRight[2])/(m_param1+0.f);
        for(int i = 0; i<m_param1; i++){
            for(int j = 0; j<m_param1;j++){
                glm::vec3 topl = glm::vec3(bottomRight[0],bottomRight[1]+(1+i)*length,bottomRight[2]+j*length);
                glm::vec3 topr = glm::vec3(bottomRight[0],bottomRight[1]+(1+i)*length,bottomRight[2]+(1+j)*length);
                glm::vec3 botl = glm::vec3(bottomRight[0],bottomRight[1]+i*length,bottomRight[2]+j*length);
                glm::vec3 botr = glm::vec3(bottomRight[0],bottomRight[1]+i*length,bottomRight[2]+(1+j)*length);
                makeTile(topl,topr,botl,botr);
            }
        }
    }
    else if(normal[0] == -1.f){
        float length = std::abs(topLeft[2]-topRight[2])/(m_param1+0.f);
        for(int i = 0; i<m_param1; i++){
            for(int j = 0; j<m_param1;j++){
                glm::vec3 topl = glm::vec3(bottomRight[0],bottomRight[1]+(1+i)*length,bottomRight[2]-j*length);
                glm::vec3 topr = glm::vec3(bottomRight[0],bottomRight[1]+(1+i)*length,bottomRight[2]-(1+j)*length);
                glm::vec3 botl = glm::vec3(bottomRight[0],bottomRight[1]+i*length,bottomRight[2]-j*length);
                glm::vec3 botr = glm::vec3(bottomRight[0],bottomRight[1]+i*length,bottomRight[2]-(1+j)*length);
                makeTile(topl,topr,botl,botr);
            }
        }
    }
    // --------
    // ----------
    if(normal[1] == 1.f){
        float length = std::abs(topLeft[0]-topRight[0])/(m_param1+0.f);
        for(int i = 0; i<m_param1; i++){
            for(int j = 0; j<m_param1;j++){
                glm::vec3 topl = glm::vec3(bottomLeft[0]-j*length,bottomLeft[1],bottomLeft[2]+(1+i)*length);
                glm::vec3 topr = glm::vec3(bottomLeft[0]-(1+j)*length,bottomLeft[1],bottomLeft[2]+(1+i)*length);
                glm::vec3 botl = glm::vec3(bottomLeft[0]-j*length,bottomLeft[1],bottomLeft[2]+i*length);
                glm::vec3 botr = glm::vec3(bottomLeft[0]-(1+j)*length,bottomLeft[1],bottomLeft[2]+i*length);
                makeTile(topl,topr,botl,botr);
            }
        }
    }
    else if(normal[1] == -1.f){
        float length = std::abs(topLeft[0]-topRight[0])/(m_param1+0.f);
        for(int i = 0; i<m_param1; i++){
            for(int j = 0; j<m_param1;j++){
                glm::vec3 topl = glm::vec3(bottomLeft[0]+j*length,bottomLeft[1],bottomLeft[2]+(1+i)*length);
                glm::vec3 topr = glm::vec3(bottomLeft[0]+(1+j)*length,bottomLeft[1],bottomLeft[2]+(1+i)*length);
                glm::vec3 botl = glm::vec3(bottomLeft[0]+j*length,bottomLeft[1],bottomLeft[2]+i*length);
                glm::vec3 botr = glm::vec3(bottomLeft[0]+(1+j)*length,bottomLeft[1],bottomLeft[2]+i*length);
                makeTile(topl,topr,botl,botr);
            }
        }
    }
}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

    //makeTile(glm::vec3(-0.5f,  0.5f, 0.5f),
     //         glm::vec3( 0.5f,  0.5f, 0.5f),
     //         glm::vec3(-0.5f, -0.5f, 0.5f),
      //        glm::vec3( 0.5f, -0.5f, 0.5f));

    // Uncomment these lines for Task 3:
    //front
    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f));
    //behind
    makeFace(glm::vec3(0.5f,  0.5f, -0.5f),
              glm::vec3( -0.5f,  0.5f, -0.5f),
              glm::vec3(0.5f, -0.5f, -0.5f),
              glm::vec3( -0.5f, -0.5f, -0.5f));
    //right
    makeFace(glm::vec3(0.5f,  0.5f, -0.5f),
              glm::vec3(0.5f,  0.5f, 0.5f),
              glm::vec3(0.5f, -0.5f, -0.5f),
              glm::vec3(0.5f, -0.5f, 0.5f));

    //left
    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f));

    //up
    makeFace(glm::vec3(-0.5f,  -0.5f, 0.5f),
              glm::vec3(0.5f,  -0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3(0.5f, -0.5f, -0.5f));

    //down
    makeFace(glm::vec3(0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3(0.5f, 0.5f, -0.5f),
              glm::vec3(-0.5f, 0.5f, -0.5f));

    // Task 4: Use the makeFace() function to make all 6 sides of the cube

}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
