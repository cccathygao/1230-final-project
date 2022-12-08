#include "cylinder.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = max(param1,1);
    m_param2 = max(param2,3);
    setVertexData();
}


void Cylinder::setVertexData() {
    // TODO for Project 5: Lights, Camera

    float inc = 360.0/m_param2;
    for(int i=0; i<m_param2; i++){
        float x1 = 0.5*cos(radians(i*inc));
        float z1 = 0.5*sin(radians(i*inc));
        float x2 = 0.5*cos(radians((i+1)*inc));
        float z2 = 0.5*sin(radians((i+1)*inc));
        setWedge(vec2(x1,z1),vec2(x2,z2));
        setBaseWedge(i*inc,(i+1)*inc);

    }

}

void Cylinder::setBaseWedge(float angle1, float angle2){
    vec3 topNormal = vec3(0.f,1.f,0.f);
    vec3 botNormal = vec3(0.f,-1.f,0.f);
    float inc = 0.5/m_param1;
    for(int i=0; i<m_param1;i++){
        float r1 = 0.5-(i*inc);
        float r2 = 0.5-((i+1)*inc);
        float outx1 = r1*cos(radians(angle1));
        float outz1 = r1*sin(radians(angle1));
        float outx2 = r1*cos(radians(angle2));
        float outz2 = r1*sin(radians(angle2));
        float inx1 = r2*cos(radians(angle1));
        float inz1 = r2*sin(radians(angle1));
        float inx2 = r2*cos(radians(angle2));
        float inz2 = r2*sin(radians(angle2));
        //botbase
        insertVec3(m_vertexData,vec3(inx2,-0.5,inz2));
        insertVec3(m_vertexData,botNormal);
        insertVec3(m_vertexData,vec3(inx1,-0.5,inz1));
        insertVec3(m_vertexData,botNormal);
        insertVec3(m_vertexData,vec3(outx1,-0.5,outz1));
        insertVec3(m_vertexData,botNormal);
        insertVec3(m_vertexData,vec3(outx1,-0.5,outz1));
        insertVec3(m_vertexData,botNormal);
        insertVec3(m_vertexData,vec3(outx2,-0.5,outz2));
        insertVec3(m_vertexData,botNormal);
        insertVec3(m_vertexData,vec3(inx2,-0.5,inz2));
        insertVec3(m_vertexData,botNormal);
        //topbase
        insertVec3(m_vertexData,vec3(inx2,0.5,inz2));
        insertVec3(m_vertexData,topNormal);
        insertVec3(m_vertexData,vec3(outx1,0.5,outz1));
        insertVec3(m_vertexData,topNormal);
        insertVec3(m_vertexData,vec3(inx1,0.5,inz1));
        insertVec3(m_vertexData,topNormal);
        insertVec3(m_vertexData,vec3(outx1,0.5,outz1));
        insertVec3(m_vertexData,topNormal);
        insertVec3(m_vertexData,vec3(inx2,0.5,inz2));
        insertVec3(m_vertexData,topNormal);
        insertVec3(m_vertexData,vec3(outx2,0.5,outz2));
        insertVec3(m_vertexData,topNormal);
    }

}



void Cylinder::setWedge(glm::vec2 leftVertex, glm::vec2 rightVertex){
    float inc = 1.f/m_param1;
    vec3 leftNormal = normalize(vec3(leftVertex[0],0,leftVertex[1]));
    vec3 rightNormal = normalize(vec3(rightVertex[0],0,rightVertex[1]));
    for(int i = 0; i<m_param1;i++){
        float topY = 0.5-inc*i;
        float botY = 0.5-inc*(i+1);
        glm::vec3 topLeft = vec3(leftVertex[0],topY,leftVertex[1]);
        glm::vec3 topRight = vec3(rightVertex[0],topY,rightVertex[1]);
        glm::vec3 botLeft = vec3(leftVertex[0], botY, leftVertex[1]);
        glm::vec3 botRight = vec3(rightVertex[0],botY,rightVertex[1]);


        insertVec3(m_vertexData,topRight);
        insertVec3(m_vertexData,rightNormal);
        insertVec3(m_vertexData,botLeft);
        insertVec3(m_vertexData,leftNormal);
        insertVec3(m_vertexData,topLeft);
        insertVec3(m_vertexData,leftNormal);

        insertVec3(m_vertexData,topRight);
        insertVec3(m_vertexData,rightNormal);
        insertVec3(m_vertexData,botRight);
        insertVec3(m_vertexData,rightNormal);
        insertVec3(m_vertexData,botLeft);
        insertVec3(m_vertexData,leftNormal);

    }
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
