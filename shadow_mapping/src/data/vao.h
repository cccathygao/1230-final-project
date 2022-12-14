#ifndef VAO_H
#define VAO_H

#include "GL/glew.h"
#include "vbo.h"
#include <memory>

class VAO
{
public:

    VAO();
    ~VAO(){};
    void initialize();
    void bind();
    void unbind();
    void bindVBO(GLuint VBOid);
    void unbindVBO();
    void setAttributes(GLuint VBOid);
    void draw(int dataSize, GLuint colorTexture, GLuint depthTexture);
    const GLuint getId() { return m_id; }

private:
    GLuint m_id;
};

#endif // VAO_H
