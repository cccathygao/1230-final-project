#include "sphere.h"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(param1,2);
    m_param2 = std::max(param2,3);
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    // Task 5: Implement the makeTile() function for a Sphere
    // Note: this function is very similar to the makeTile() function for Cube,
    //       but the normals are calculated in a different way!
    insertVec3(m_vertexData,topLeft);
    insertVec3(m_vertexData,glm::normalize(topLeft));
    insertVec3(m_vertexData,bottomLeft);
    insertVec3(m_vertexData,glm::normalize(bottomLeft));
    insertVec3(m_vertexData,bottomRight);
    insertVec3(m_vertexData,glm::normalize(bottomRight));
    insertVec3(m_vertexData,topLeft);
    insertVec3(m_vertexData,glm::normalize(topLeft));
    insertVec3(m_vertexData,bottomRight);
    insertVec3(m_vertexData,glm::normalize(bottomRight));
    insertVec3(m_vertexData,topRight);
    insertVec3(m_vertexData,glm::normalize(topRight));
}

glm::vec3 cartesian(float r, float phi, float theta){
    float x = r*glm::sin(phi)*glm::sin(theta);
    float y = r*glm::cos(phi);
    float z = r*glm::sin(phi)*glm::cos(theta);
    return glm::vec3(x,y,z);
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    // Task 6: create a single wedge of the sphere using the
    //         makeTile() function you implemented in Task 5
    // Note: think about how param 1 comes into play here!
    float length = 180.f/m_param1;
    for(int i =0; i<m_param1;i++){
        float topphi = glm::radians(i*length);
        float botphi = glm::radians((i+1)*length);
        glm::vec3 topL = cartesian(0.5,topphi,currentTheta);
        glm::vec3 botL = cartesian(0.5,botphi,currentTheta);
        glm::vec3 botR = cartesian(0.5,botphi,nextTheta);
        glm::vec3 topR = cartesian(0.5,topphi,nextTheta);
        makeTile(topL,topR,botL,botR);
    }

}

void Sphere::makeSphere() {
    // Task 7: create a full sphere using the makeWedge() function you
    //         implemented in Task 6
    // Note: think about how param 2 comes into play here!
    float length = 360.f/m_param2;
    for(int i=0; i<m_param2;i++){
        makeWedge(glm::radians(length*i),glm::radians(length*(i+1)));
    }

}

void Sphere::setVertexData() {
    // Uncomment these lines to make a wedge for Task 6, then comment them out for Task 7:

    //float thetaStep = glm::radians(360.f / m_param2);
    //float currentTheta = 0 * thetaStep;
    //float nextTheta = 1 * thetaStep;
    //makeWedge(currentTheta, nextTheta);

    // Uncomment these lines to make sphere for Task 7:

    makeSphere();
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
