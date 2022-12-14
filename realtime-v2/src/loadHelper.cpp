#include "glm/gtx/transform.hpp"
#include "realtime.h"
#include "settings.h"
#include <iostream>
#include "OBJ_Loader.h"

using namespace glm;

/**
 * This is the class where VBO and VAO handling methods are located
 * responsible for initializing and updating data
 */
void Realtime::setupCamera(){
    const SceneCameraData& cameraData = metaData.cameraData;
    camera = Camera(cameraData.pos,cameraData.look,cameraData.up,cameraData.heightAngle,
                  cameraData.aperture,cameraData.focalLength, size().height(), size().width(),
                  settings.nearPlane, settings.farPlane);
}

/**
 * @brief Realtime::setupShapesVBO
 * This function creates 4 vbos and 4 vaos for each primitive shape
 * the vbos contain vertiex postions and normals in the object space
 */

void Realtime::setupShapesVBO(){
    glGenBuffers(4,&shape_vbos[0]);
    glGenVertexArrays(4,&shape_vaos[0]);
    for(int i=0; i<4; i++){
        glBindVertexArray(shape_vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER,shape_vbos[i]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),reinterpret_cast<void *>(sizeof(GLfloat)*3));
    }

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

/**
 * @brief Realtime::updateShapesVBO
 * This function modifies the content of vbo buffers according to the changes in the
 * tessellation of primitive shapes
 */
void Realtime::updateShapesVBO(){
    if(!initialized){
        return;
    }
    for(int i=0; i<4; i++){
        glBindBuffer(GL_ARRAY_BUFFER,shape_vbos[i]);
        Shape& currShape = shape((PrimitiveType)i);
        currShape.updateParams(12,12);
        std::vector<GLfloat> &data = currShape.generateShape();
        glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(GLfloat),data.data(), GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

SettingType Realtime::settingChangeType(){
    bool camera = (settings.farPlane!=farPlane) || (settings.nearPlane!=nearPlane);
    bool shape = (settings.shapeParameter1!=shapeParameter1) || (settings.shapeParameter2!=shapeParameter2);
    nearPlane = settings.nearPlane;
    farPlane = settings.farPlane;
    shapeParameter1 = settings.shapeParameter1;
    shapeParameter2 = settings.shapeParameter2;
    if(camera && shape){
        return SettingType::CAMERA_AND_SHAPE;
    }
    if(camera){
        return SettingType::CAMERA_ONLY;
    }
    return SettingType::SHAPE_ONLY;
}


Shape& Realtime::shape(PrimitiveType type){
    if(type == PrimitiveType::PRIMITIVE_CONE){
        return cone;
    }
    if(type == PrimitiveType::PRIMITIVE_CUBE){
        return cube;
    }
    if(type == PrimitiveType::PRIMITIVE_CYLINDER){
        return cylinder;
    }
    return sphere;
}

int Realtime::shapeSize(PrimitiveType type){
    return shape(type).generateShape().size();
}

void Realtime::loadOBJ(){
//    std::vector<float> buffer;
    m_buffer.clear();
    objl::Loader loader;
    loader.LoadFile("Falcon.obj");
    std::vector<objl::Mesh> meshes = loader.LoadedMeshes;
    for(int i = 0; i<meshes.size();i++){
        std::vector<objl::Vertex> vertices = meshes[i].Vertices;
        for(int j=0; j<vertices.size(); j++){
            m_buffer.push_back(vertices[j].Position.X);
            m_buffer.push_back(vertices[j].Position.Y);
            m_buffer.push_back(vertices[j].Position.Z);
            m_buffer.push_back(vertices[j].Normal.X);
            m_buffer.push_back(vertices[j].Normal.Y);
            m_buffer.push_back(vertices[j].Normal.Z);
            m_buffer.push_back(vertices[j].TextureCoordinate.X);
            m_buffer.push_back(vertices[j].TextureCoordinate.Y);
            std::cout << vertices[j].Position.X << vertices[j].Position.Y << vertices[j].Position.Z << std::endl;
            std::cout << std::endl;
        }
    }
    vert_size = m_buffer.size()/3;

    // set VBO data
    m_bird_vbo.setData(m_buffer);

    // set VAO attributes
    m_bird_vao.setAttributes(m_bird_vbo.getId());
}

void Realtime::paintBird(){
    //mat4 vp = camera.getProjMatrix()*camera.getViewMatrix();

    //vec3 cAmbient = vec3(153/255.f,76/255.f,0);
    //vec3 cDiffuse = vec3(153/255.f,76/255.f,0);
    vec3 cAmbient = vec3(153/255.f,76/255.f,0);
    vec3 cDiffuse = vec3(1);
    vec3 cSpecular = vec3(0);
    float shininess = 0;

    glUseProgram(m_phong_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader,"modelMat"),1,GL_FALSE,&bird_ctm[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader,"worldNormalMat"),1,GL_FALSE,&normal_ctm[0][0]);

    // depth map
    GLint depthTextureLocation = glGetUniformLocation(m_phong_shader, "depthMap");
    glUniform1i(depthTextureLocation, 1);

    // light space transformation matrix
    GLint lightSpaceMatrixLocation = glGetUniformLocation(m_phong_shader, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &m_lightSpaceMatrix[0][0]);


    glUniform3fv(glGetUniformLocation(m_phong_shader,"cAmbient"),1,&cAmbient[0]);
    glUniform3fv(glGetUniformLocation(m_phong_shader,"cDiffuse"),1,&cDiffuse[0]);
    glUniform3fv(glGetUniformLocation(m_phong_shader,"cSpecular"),1,&cSpecular[0]);
    glUniform1f(glGetUniformLocation(m_phong_shader,"shininess"),shininess);


    m_bird_vao.draw(m_buffer.size(), -1, m_depth_texture);
    glUseProgram(0);
}
void Realtime::paintLand(){
    //mat4 vp = camera.getProjMatrix()*camera.getViewMatrix();
    mat4 model = mat4(1000,0,0,0,
                 0,1,0,0,
                 0,0,1000,0,
                 0,0,0,1);
    mat4 itctm = inverse(transpose(model));
    //vec3 cAmbient = vec3(153/255.f,76/255.f,0);
    //vec3 cDiffuse = vec3(153/255.f,76/255.f,0);
    vec3 cAmbient = vec3(0.63,0.86,0.52);
    vec3 cDiffuse = vec3(0.42,0.57,0.35);
    vec3 cSpecular = vec3(0);
    float shininess = 0;

    glUseProgram(m_phong_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader,"modelMat"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader,"worldNormalMat"),1,GL_FALSE,&itctm[0][0]);

    // depth map
    GLint depthTextureLocation = glGetUniformLocation(m_phong_shader, "depthMap");
    glUniform1i(depthTextureLocation, 1);

    // light space transformation matrix
    GLint lightSpaceMatrixLocation = glGetUniformLocation(m_phong_shader, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &m_lightSpaceMatrix[0][0]);


    glUniform3fv(glGetUniformLocation(m_phong_shader,"cAmbient"),1,&cAmbient[0]);
    glUniform3fv(glGetUniformLocation(m_phong_shader,"cDiffuse"),1,&cDiffuse[0]);
    glUniform3fv(glGetUniformLocation(m_phong_shader,"cSpecular"),1,&cSpecular[0]);
    glUniform1f(glGetUniformLocation(m_phong_shader,"shininess"),shininess);


    glBindVertexArray(shape_vaos[static_cast<int>(PrimitiveType::PRIMITIVE_CUBE)]);

    // shadow mapping
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);

    glDrawArrays(GL_TRIANGLES,0,shapeSize(PrimitiveType::PRIMITIVE_CUBE)/6);
    glUseProgram(0);

    model = mat4(5,0,0,0,
                 0,5,0,0,
                 0,0,5,0,
                 0,0,0,1);
    model = mat4(1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,5.5,50,1)*model;

    itctm = inverse(transpose(model));
    //vec3 cAmbient = vec3(153/255.f,76/255.f,0);
    //vec3 cDiffuse = vec3(153/255.f,76/255.f,0);
    cAmbient = vec3(0);
    cDiffuse = vec3(0.8,0.6,0.6);
    cSpecular = vec3(1);
    shininess = 15;

    glUseProgram(m_phong_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader,"modelMat"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader,"worldNormalMat"),1,GL_FALSE,&itctm[0][0]);



    glUniform3fv(glGetUniformLocation(m_phong_shader,"cAmbient"),1,&cAmbient[0]);
    glUniform3fv(glGetUniformLocation(m_phong_shader,"cDiffuse"),1,&cDiffuse[0]);
    glUniform3fv(glGetUniformLocation(m_phong_shader,"cSpecular"),1,&cSpecular[0]);
    glUniform1f(glGetUniformLocation(m_phong_shader,"shininess"),shininess);



    glBindVertexArray(shape_vaos[static_cast<int>(PrimitiveType::PRIMITIVE_CYLINDER)]);
    glDrawArrays(GL_TRIANGLES,0,shapeSize(PrimitiveType::PRIMITIVE_CYLINDER)/6);
    glUseProgram(0);
}

void Realtime::initializeScene(){
    //initialize camera

    vec4 birdPos = vec4(0,15,15,1);

    vec4 pos = vec4(0,30,0,1);
    vec4 look = normalize(birdPos-pos);
    vec4 up = vec4(0,1,0,0);
    float heightAngle = glm::radians(30.f);

    camera = Camera(pos,look,up,heightAngle,0,0,size().height(),size().width(),10,100.f);

    //initialize light sources
    SceneLightData light1;
    light1.id = 0;
    light1.type = LightType::LIGHT_DIRECTIONAL;
    light1.dir = vec4(0,-1,1,0);
    light1.color = vec4(1.f);

    lights.push_back(light1);

    // shadow mapping, set m_lightSpaceMatrix
    lightSpaceTransform(lights[0]);

    parseGlobal();
    parseLights();

}
