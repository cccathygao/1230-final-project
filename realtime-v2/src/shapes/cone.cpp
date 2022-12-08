#include "cone.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = max(param1,1);
    m_param2 = max(param2,3);
    setVertexData();
}


void Cone::setVertexData() {
    // TODO for Project 5: Lights, Camera
    float inc = 360.0/m_param2;
    for(int i=0; i<m_param2; i++){
        setWedge(radians(i*inc),radians((i+1)*inc));
        setBaseWedge(i*inc,(i+1)*inc);


    }
}

vec3 Cone::normalFinder(float x, float y, float z, float r, float theta){
    if(x==0 && z == 0){
        x =  cos(theta+radians(360.f/m_param2*0.5));
        z =  sin(theta+radians(360.f/m_param2*0.5));
    }
    vec2 temp = normalize(vec2(x,z));
    return normalize(vec3(temp[0]*2.f,1.f,temp[1]*2.f));
}

void Cone::setWedge(float theta1, float theta2){
    float inc = 1.f/m_param1;

    for(int i = 0; i<m_param1;i++){
        float topY = -0.5+inc*(i+1);
        float botY = -0.5+inc*i;
        float r1 = (0.5-topY)/2.f;
        float r2 = (0.5-botY)/2.f;
        float topXLeft = r1*cos(theta1);
        float topZLeft = r1*sin(theta1);
        float botXLeft = r2*cos(theta1);
        float botZLeft = r2*sin(theta1);
        float topXRight = r1*cos(theta2);
        float topZRight = r1*sin(theta2);
        float botXRight = r2*cos(theta2);
        float botZRight = r2*sin(theta2);

        glm::vec3 topLeft = vec3(topXLeft,topY,topZLeft);
        glm::vec3 topRight = vec3(topXRight,topY,topZRight);
        glm::vec3 botLeft = vec3(botXLeft, botY, botZLeft);
        glm::vec3 botRight = vec3(botXRight,botY,botZRight);
        vec3 topLeftNormal = normalFinder(topXLeft,topY,topZLeft,r1,theta1);
        vec3 topRightNormal = normalFinder(topXRight,topY,topZRight,r1,theta2);
        vec3 botLeftNormal = normalFinder(botXLeft,botY,botZLeft,r2,theta1);
        vec3 botRightNormal = normalFinder(botXRight,botY,botZRight,r2,theta2);

        insertVec3(m_vertexData,topRight);
        insertVec3(m_vertexData,topRightNormal);
        insertVec3(m_vertexData,botLeft);
        insertVec3(m_vertexData,botLeftNormal);
        insertVec3(m_vertexData,topLeft);
        insertVec3(m_vertexData,topLeftNormal);
        insertVec3(m_vertexData,topRight);
        insertVec3(m_vertexData,topRightNormal);
        insertVec3(m_vertexData,botRight);
        insertVec3(m_vertexData,botRightNormal);
        insertVec3(m_vertexData,botLeft);
        insertVec3(m_vertexData,botLeftNormal);
    }
}
void Cone::setBaseWedge(float angle1, float angle2){
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

    }

}

void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
