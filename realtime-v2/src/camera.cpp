#include <iostream>
#include <ostream>
#include <stdexcept>
#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <cmath>

void printMat(glm::mat4 mat);

Camera::Camera(glm::vec4 pos,glm::vec4 look,glm::vec4 up,float heightAngle,float aperture,float focalLength, int imageH, int imageW,
               float near, float far){
    this->near = near;
    this->far = far;
    this->pos = pos;
    this->look = look;
    this->up = up;
    this->heightAngle = heightAngle;
    this->aperture = aperture;
    this->focalLength = focalLength;
    this->viewMatrix = calculateViewMatrix(pos,look,up);
    this->aspectRatio = (imageW+0.f)/imageH;
    this->inverseView = glm::inverse(viewMatrix);
    this->projMatrix = calculateProjMatrix(near, far, heightAngle, getWidthAngle());

}
Camera::Camera(){

}
glm::mat4& Camera::getViewMatrix()  {
    // Optional TODO: implement the getter or make your own design
    return viewMatrix;
}
glm::mat4& Camera::getInverseV(){
    return inverseView;
}

glm::mat4 Camera::calculateViewMatrix(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {
    // Task 7: return the camera view matrix
    auto m_t = glm::mat4(1.f,0.f,0.f,0.f, 0.f,1.f,0.f,0.f,  0.f,0.f,1.f,0.f, -pos[0],-pos[1],-pos[2],1.f);
    auto w = -glm::normalize(look);
    auto v = glm::normalize(up-glm::dot(up,w)*w);
    auto u = glm::cross(v,w);

    auto m_r = glm::mat4(u[0],v[0],w[0],0.f,  u[1],v[1],w[1],0.f, u[2],v[2],w[2],0.f, 0.f,0.f,0.f,1.f);
    return m_r*m_t;
}
glm::mat4 Camera::calculateProjMatrix(float near, float far, float heightAngle, float widthAngle){
    float c = -near/far;
    glm::mat4 remap = glm::mat4(1.f,0.f,0.f,0.f,
                                0.f,1.f,0.f,0.f,
                                0.f,0.f,-2.f,0.f,
                                0.f,0.f,-1.f,1.f);
    glm::mat4 unhinge = glm::mat4(1.f,0.f,0.f,0.f,
                                  0.f,1.f,0.f,0.f,
                                  0.f,0.f,1.f/(1.f+c),-1.f,
                                  0.f,0.f,-c/(1.f+c),0.f);
    glm::mat4 scale = glm::mat4(1.f/(far*glm::tan(widthAngle/2.f)),0.f,0.f,0.f,
                                0.f,1.f/(far*glm::tan(heightAngle/2.f)),0.f,0.f,
                                0.f,0.f,1.f/far,0.f,
                                0.f,0.f,0.f,1.f);
    this->projMatrix =remap*unhinge*scale;
    return projMatrix;
}
glm::mat4& Camera::getProjMatrix(){
    return projMatrix;
}

float Camera::getAspectRatio()  {
    // Optional TODO: implement the getter or make your own design
    return aspectRatio;
}

float Camera::getHeightAngle()  {
    // Optional TODO: implement the getter or make your own design
    return heightAngle;
}

float Camera::getFocalLength()  {
    // Optional TODO: implement the getter or make your own design
    return focalLength;
}

float Camera::getAperture()  {
    // Optional TODO: implement the getter or make your own design
    return aperture;
}

float Camera::getViewPlaneHeight() {
    return 2.f*glm::tan(heightAngle/2);
}
float Camera::getViewPlaneWidth() {
    return aspectRatio*getViewPlaneHeight();
}
float Camera::getWidthAngle() {
    return aspectRatio*heightAngle;//2.f*(glm::atan(getViewPlaneWidth()/(2.f*getK())));
}
float Camera::getK(){
    return 1.f;
}
glm::vec4 Camera::getPos() {
    return pos;
}

glm::vec4 Camera::getLook(){
    return look;
}
glm::vec4 Camera::getUp(){
    return up;
}

void Camera::setNear(float n){
    this->near = n;
}
void Camera::setFar(float f){
    this->far = f;
}
void Camera::setLook(glm::vec4 look){
    this->look = look;
}
void Camera::setUp(glm::vec4 up){
    this->up = up;
}
void Camera::setPos(glm::vec4 pos){
    this->pos = pos;
}
void Camera::setAspectRatio(int w, int h){
    this->aspectRatio = (w+0.f)/h;
}
void Camera::modifyProj() {
    calculateProjMatrix(near,far,heightAngle,getWidthAngle());
}

void Camera::modifyView() {
    this->viewMatrix = calculateViewMatrix(pos,look,up);
    this->inverseView = glm::inverse(viewMatrix);
}


void printMat(glm::mat4 mat){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            std::cout<<mat[j][i]<< "  ";
        }
        std::cout<<std::endl;
    }
}
