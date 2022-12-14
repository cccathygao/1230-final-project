#ifndef VBO_H
#define VBO_H

#include "GL/glew.h"
#include <vector>

class VBO
{
public:
    VBO();
    ~VBO(){};
    void initialize();
    void bind();
    void unbind();
    void setData(std::vector<float> &data);
    const GLuint getId(){ return m_id; }

private:
    GLuint m_id;
};

#endif // VBO_H
