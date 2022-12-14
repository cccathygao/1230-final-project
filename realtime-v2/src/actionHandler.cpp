#include "realtime.h"
#include "settings.h"
#include <iostream>

using namespace glm;

void Realtime::moveCamera(float deltaTime){
    glm::vec4 pos = camera.getPos();
    if(m_keyMap[Qt::Key_W]){
        pos += 5.f*deltaTime*glm::normalize(camera.getLook());
    }
    if(m_keyMap[Qt::Key_S]){
        pos += 5.f*deltaTime*glm::normalize(-camera.getLook());
    }
    if(m_keyMap[Qt::Key_A]){
        pos += 5.f*deltaTime*glm::normalize(glm::vec4(glm::cross(glm::vec3(camera.getUp()),glm::vec3(camera.getLook())),0.f));
    }
    if(m_keyMap[Qt::Key_D]){
        pos += 5.f*deltaTime*glm::normalize(glm::vec4(glm::cross(glm::vec3(camera.getLook()),glm::vec3(camera.getUp())),0.f));
    }
    if(m_keyMap[Qt::Key_Space]){
        pos += 5.f*deltaTime*glm::vec4(0.f,1.f,0.f,0.f);
    }
    if(m_keyMap[Qt::Key_Control]){
        pos += 5.f*deltaTime*glm::vec4(0.f,-1.f,0.f,0.f);
    }

    camera.setPos(pos);
    camera.modifyView();
}

glm::mat3 rotateMatrix(vec3 axis, float angle){
    float c = cos(radians(angle));
    float s = sin(radians(angle));
    glm::mat3 m_rt = glm::mat3(c+pow(axis[0],2)*(1-c), axis[0]*axis[1]*(1-c)+axis[2]*s, axis[0]*axis[2]*(1-c)-axis[1]*s,
                                axis[0]*axis[1]*(1-c)-axis[2]*s, c+pow(axis[1],2)*(1-c), axis[1]*axis[2]*(1-c)+axis[0]*s,
                                axis[0]*axis[2]*(1-c)+axis[1]*s, axis[1]*axis[2]*(1-c)-axis[0]*s, c+pow(axis[2],2)*(1-c));
    return m_rt;
}

void Realtime::rotateCamera(float deltaX, float deltaY){
    float x_angle = -180*deltaX/size().width();
    float y_angle = 180*deltaY/size().height();

    //rotation around Y axis
    vec3 axis = vec3(0.f,1.f,0.f);
    mat3 m_rt1 = rotateMatrix(axis,x_angle);
    axis = normalize(cross(vec3(camera.getUp()),vec3(camera.getLook())));
    mat3 m_rt2 = rotateMatrix(axis,y_angle);
    camera.setLook(normalize(vec4(m_rt2*m_rt1*vec3(camera.getLook()),0.f)));
    camera.modifyView();
}
