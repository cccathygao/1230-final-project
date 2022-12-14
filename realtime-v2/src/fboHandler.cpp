#include "realtime.h"
#include "settings.h"
#include <iostream>


void Realtime::makeFBO(){
    glGenTextures(1,&depth_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT24,fbo_width,fbo_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glGenTextures(1,&color_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,fbo_width,fbo_height,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(m_fbo_shader);
    glUniform1i(glGetUniformLocation(m_fbo_shader,"depthTex"),0);
    glUniform1i(glGetUniformLocation(m_fbo_shader,"colorTex"),1);
    glUseProgram(0);


    glGenFramebuffers(1,&m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,color_texture,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depth_texture,0);

    glBindFramebuffer(GL_FRAMEBUFFER,default_fbo);
}

void Realtime::paintTexture(GLuint m_texture){
    glUseProgram(m_fbo_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,depth_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,color_texture);
    glm::vec2 pixelsize = glm::vec2(1.f/(size().width()+0.f),1.f/(size().height()+0.f));
    glUniform2fv(glGetUniformLocation(m_fbo_shader,"pixelSize"),1,&pixelsize[0]);
    glUniform1f(glGetUniformLocation(m_fbo_shader,"far"),100);
    glUniform1f(glGetUniformLocation(m_fbo_shader,"fp"),settings.focalPoint);
    glUniform1f(glGetUniformLocation(m_fbo_shader,"fs"),settings.focalScale);
    glUniform1i(glGetUniformLocation(m_fbo_shader,"dofOn"),settings.dof);
    glBindVertexArray(fullscreen_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Realtime::setFullscreenquad(){
    std::vector<GLfloat> fullscreen_quad_data =
    {
        -1.f,  1.f, 0.f,
         0.f,  1.f,
        -1.f, -1.f, 0.f,
         0.f,  0.f,
         1.f, -1.f, 0.f,
         1.f,  0.f,
         1.f,  1.f, 0.f,
         1.f,  1.f,
        -1.f,  1.f, 0.f,
         0.f,  1.f,
         1.f, -1.f, 0.f,
         1.f,  0.f
    };

    glGenBuffers(1, &fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &fullscreen_vao);
    glBindVertexArray(fullscreen_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(sizeof(GLfloat)*3));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
