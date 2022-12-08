#include "realtime.h"
#include "settings.h"
#include <iostream>


/**
 * @brief This class contains helper function for the paintGL call
 * mostly just passing uniform data
 */

void Realtime::paintScene(){
    glUseProgram(m_shader);
    parseGlobal();
    parseLights();
    for(RenderShapeData &shapeData : metaData.shapes){
        glBindVertexArray(shape_vaos[static_cast<int>(shapeData.primitive.type)]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader,"modelMat"),1,GL_FALSE,&shapeData.ctm[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader,"worldNormalMat"),1,GL_FALSE,&shapeData.invTranspCtm[0][0]);
        parseShapes(shapeData.primitive);
        glDrawArrays(GL_TRIANGLES,0,shapeSize(shapeData.primitive.type)/3);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

void Realtime::parseLights(){
    std::vector<SceneLightData> &lights = metaData.lights;
    int i;
    for(i=0;i<lights.size();i++){
        std::string scolor = "lights[" + std::to_string(i) + "].color";
        std::string stype = "lights[" + std::to_string(i) + "].type";
        std::string spos = "lights[" + std::to_string(i) + "].pos";
        std::string sdir = "lights[" + std::to_string(i) + "].dir";
        std::string spenumbra = "lights[" + std::to_string(i) + "].penumbra";
        std::string sangle = "lights[" + std::to_string(i) + "].angle";
        std::string sfunction = "lights[" + std::to_string(i) + "].function";
        glUniform3fv(glGetUniformLocation(m_shader, scolor.c_str()),1,&lights[i].color[0]);
        if(lights[i].type==LightType::LIGHT_DIRECTIONAL){
            glUniform1i(glGetUniformLocation(m_shader,stype.c_str()),0);
            glUniform3fv(glGetUniformLocation(m_shader, sdir.c_str()),1,&lights[i].dir[0]);
        }
        else if(lights[i].type==LightType::LIGHT_POINT){
            glUniform1i(glGetUniformLocation(m_shader,stype.c_str()),1);
            glUniform3fv(glGetUniformLocation(m_shader, spos.c_str()),1,&lights[i].pos[0]);
            glUniform3fv(glGetUniformLocation(m_shader, sfunction.c_str()),1,&lights[i].function[0]);
        }
        else{
            glUniform1i(glGetUniformLocation(m_shader,stype.c_str()),2);
            glUniform3fv(glGetUniformLocation(m_shader, spos.c_str()),1,&lights[i].pos[0]);
            glUniform3fv(glGetUniformLocation(m_shader, sdir.c_str()),1,&lights[i].dir[0]);
            glUniform1f(glGetUniformLocation(m_shader,spenumbra.c_str()),lights[i].penumbra);
            glUniform1f(glGetUniformLocation(m_shader,sangle.c_str()),lights[i].angle);
            glUniform3fv(glGetUniformLocation(m_shader, sfunction.c_str()),1,&lights[i].function[0]);
        }
    }
    glUniform1i(glGetUniformLocation(m_shader,"nlights"),i);
}

void Realtime::parseShapes(ScenePrimitive& primitive){
    glUniform1f(glGetUniformLocation(m_shader,"shininess"),primitive.material.shininess);
    glUniform3fv(glGetUniformLocation(m_shader,"cAmbient"),1,&primitive.material.cAmbient[0]);
    glUniform3fv(glGetUniformLocation(m_shader,"cDiffuse"),1,&primitive.material.cDiffuse[0]);
    glUniform3fv(glGetUniformLocation(m_shader,"cSpecular"),1,&primitive.material.cSpecular[0]);
}

void Realtime::parseGlobal(){
    glUniform1f(glGetUniformLocation(m_shader,"ka"),metaData.globalData.ka);
    glUniform1f(glGetUniformLocation(m_shader,"kd"),metaData.globalData.kd);
    glUniform1f(glGetUniformLocation(m_shader,"ks"),metaData.globalData.ks);
    glm::mat4 &view = camera.getViewMatrix();
    glm::mat4 &proj = camera.getProjMatrix();
    glm::vec3 posCam = camera.getPos();
    glUniformMatrix4fv(glGetUniformLocation(m_shader,"viewMat"),1,GL_FALSE,&view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader,"projMat"),1,GL_FALSE,&proj[0][0]);
    glUniform3fv(glGetUniformLocation(m_shader,"posCam"),1,&posCam[0]);
}
