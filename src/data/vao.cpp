#include "vao.h"
#include <iostream>

VAO::VAO()
{
}

void VAO::initialize(){
    glGenVertexArrays(1, &m_id);
};

void VAO::bind(){
    glBindVertexArray(m_id);
}

void VAO::bindVBO(GLuint VBOid){
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
}

void VAO::unbind(){
    glBindVertexArray(0);
}

void VAO::unbindVBO(){
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void VAO::setAttributes(GLuint VBOid){
    bind();
    bindVBO(VBOid);
    // 0: position, 1: normal, 2: uv
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    unbindVBO();
    unbind();
};

void VAO::draw(int dataSize, GLuint colorTexture, GLuint depthTexture){
    bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    // debug depth map
    // shadow mapping
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glDrawArrays(GL_TRIANGLES, 0, dataSize / 8); // 3 for pos, 3 for normal, 2 for uv
    glBindTexture(GL_TEXTURE_2D, 0);
    unbind();
}
