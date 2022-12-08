#include <stdexcept>
#include "camera.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

void Camera::init(SceneCameraData data, float aspectRatio) {

    m_look = glm::normalize(glm::vec3(data.look));
    m_up = glm::normalize(glm::vec3(data.up));
    m_pos = data.pos;

    m_heightAngle = data.heightAngle;
    m_aspectRatio = aspectRatio;

    // TODO: update with setViewMatrix
    setViewMatrix(glm::normalize(m_look), glm::normalize(m_up), glm::vec3(m_pos));
}

float Camera::getAspectRatio() const {
    return m_aspectRatio;
}

float Camera::getHeightAngle() const {
    return m_heightAngle;
}

glm::mat4 Camera::getViewMatrix() const {
    return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const{
    return m_projectionMatrix;
}

glm::vec4 Camera::getCameraPos() const{
    return m_pos;
}

glm::vec3 Camera::getCameraLook() const{
    return m_look;
}

glm::vec3 Camera::getCameraUp() const{
    return m_up;
}

void Camera::setProjectionMatrix(){
//    float h = m_far * glm::tan(m_heightAngle / 2.f);
//    float w = m_aspectRatio * h;
//    m_scaleMatrix = glm::mat4(glm::vec4(1.f/w, 0.f, 0.f, 0.f),
//                              glm::vec4(0.f, 1.f/h, 0.f, 0.f),
//                              glm::vec4(0.f, 0.f, 1.f/m_far, 0.f),
//                              glm::vec4(0.f, 0.f, 0.f, 1.f));
//    float c = -m_near / m_far;
//    m_unhingeMatrix = glm::mat4(glm::vec4(1.f, 0.f, 0.f, 0.f),
//                                glm::vec4(0.f, 1.f, 0.f, 0.f),
//                                glm::vec4(0.f, 0.f, 1.f/(1+c), -1.f),
//                                glm::vec4(0.f, 0.f, -c/(1+c), 0.f));

//    glm::mat4 mat = glm::mat4(glm::vec4(1.f, 0.f, 0.f, 0.f),
//                              glm::vec4(0.f, 1.f, 0.f, 0.f),
//                              glm::vec4(0.f, 0.f, -2.f, 0.f),
//                              glm::vec4(0.f, 0.f, -1.f, 1.f));

//    m_projectionMatrix = mat * m_unhingeMatrix * m_scaleMatrix;

//    m_projectionMatrix = glm::perspective(m_heightAngle, m_aspectRatio, m_near, m_far);

    float inv_tan = 1.f / glm::tan(m_heightAngle / 2.f);
    m_projectionMatrix = glm::mat4(0.f);
    m_projectionMatrix[0][0] = inv_tan / m_aspectRatio;
    m_projectionMatrix[1][1] = inv_tan;
    m_projectionMatrix[2][2] = (m_near + m_far) / (m_near - m_far);
    m_projectionMatrix[2][3] = -1.f;
    m_projectionMatrix[3][2] = (2 * m_near * m_far) / (m_near - m_far);
}

void Camera::setNear(float near){
    m_near = near;
}

void Camera::setFar(float far){
    m_far = far;
}

void Camera::setAspectRatio(float aspectRatio){
    m_aspectRatio = aspectRatio;
}

void Camera::setViewMatrix(glm::vec3 look, glm::vec3 up, glm::vec3 pos){

    m_look = look;
    m_up = up;
    m_pos = glm::vec4(pos, 1.f);

    glm::mat4 T = glm::mat4(1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            -pos[0], -pos[1], -pos[2], 1.f);

    glm::vec3 w = -glm::normalize(look);
    glm::vec3 v = glm::normalize(up - glm::dot(up, w) * w);
    glm::vec3 u = glm::cross(v, w);

    glm::mat4 R = glm::mat4(u[0], v[0], w[0], 0.f,
                            u[1], v[1], w[1], 0.f,
                            u[2], v[2], w[2], 0.f,
                            0.f, 0.f, 0.f, 1.f);
    m_viewMatrix = R*T;
}
