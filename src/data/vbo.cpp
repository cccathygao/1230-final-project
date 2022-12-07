#include "vbo.h"
#include <iostream>

VBO::VBO()
{
}

// generate VBO
void VBO::initialize(){
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);
    m_id = vbo_id;
}

// bind VBO
void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

// unbind VBO
void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::setData(std::vector<float> &data){
    bind();
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
    unbind();
}
