#include "glm/gtx/transform.hpp"
#include "realtime.h"
#include "settings.h"
#include <iostream>
#include "OBJ_Loader.h"
#include "terraingenerator.h"

using namespace glm;
void invertX(std::vector<float> &verts);
void invertY(std::vector<float> &verts);
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
    std::vector<float> buffer;
    objl::Loader loader;
    loader.LoadFile("Falcon.obj");
    std::vector<objl::Mesh> meshes = loader.LoadedMeshes;
    for(int i = 0; i<meshes.size();i++){
        std::vector<objl::Vertex> vertices = meshes[i].Vertices;
        for(int j=0; j<vertices.size(); j++){
            buffer.push_back(vertices[j].Position.X);
            buffer.push_back(vertices[j].Position.Y);
            buffer.push_back(vertices[j].Position.Z);
            buffer.push_back(vertices[j].Normal.X);
            buffer.push_back(vertices[j].Normal.Y);
            buffer.push_back(vertices[j].Normal.Z);
            buffer.push_back(vertices[j].TextureCoordinate.X);
            buffer.push_back(vertices[j].TextureCoordinate.Y);
        }
    }
    vert_size = buffer.size()/3;
    glGenBuffers(1,&birdVBO);
    glGenVertexArrays(1,&birdVAO);
    glBindVertexArray(birdVAO);

    glBindBuffer(GL_ARRAY_BUFFER, birdVBO);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(GLfloat), &buffer[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),reinterpret_cast<void *>(6));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

}

void Realtime::paintBird(){
    //mat4 vp = camera.getProjMatrix()*camera.getViewMatrix();

    //vec3 cAmbient = vec3(153/255.f,76/255.f,0);
    //vec3 cDiffuse = vec3(153/255.f,76/255.f,0);
    vec3 cAmbient = vec3(0);
    vec3 cDiffuse = vec3(0.3);
    vec3 cSpecular = vec3(0);
    float shininess = 0;

    glUseProgram(m_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_shader,"modelMat"),1,GL_FALSE,&bird_ctm[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader,"worldNormalMat"),1,GL_FALSE,&normal_ctm[0][0]);


    glUniform3fv(glGetUniformLocation(m_shader,"cAmbient"),1,&cAmbient[0]);
    glUniform3fv(glGetUniformLocation(m_shader,"cDiffuse"),1,&cDiffuse[0]);
    glUniform3fv(glGetUniformLocation(m_shader,"cSpecular"),1,&cSpecular[0]);
    glUniform1f(glGetUniformLocation(m_shader,"shininess"),shininess);



    glBindVertexArray(birdVAO);
    glDrawArrays(GL_TRIANGLES,0,vert_size);
    glUseProgram(0);
}

void Realtime::paintTerrain(){
    mat4 model = mat4(300,0,0,0,
                   0,50,0,0,
                   0,0,300,0,
                   0,0,0,1);
    mat4 proj = camera.getProjMatrix();
    mat4 viewMat = camera.getViewMatrix();
    mat4 itctm = inverse(transpose(model));

    itctm = inverse(transpose(model));
    glUseProgram(terrain_shader);

    glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"modelMat"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"projMat"),1,GL_FALSE,&proj[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"viewMat"),1,GL_FALSE,&viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"worldNormalMat"),1,GL_FALSE,&itctm[0][0]);

    glBindVertexArray(terrain_vao);
    glDrawArrays(GL_TRIANGLES,0,terrain_size);
    glUseProgram(0);

    std::vector<float> deltaPos = {0, -0.8, 0.8};
/*
    for (int i = 0; i < deltaPos.size(); ++i) {
        for (int j = 0; j < deltaPos.size(); ++j) {
            glUseProgram(terrain_shader);
            model = mat4(300,0,0,0,
                                             0,50,0,0,
                                             0,0,300,0,
                                             0,0,0,1)
                                       *mat4(1, 0, 0, 0,
                                             0, 1, 0, 0,
                                             0, 0, 1, 0,
                                             deltaPos[i], 0, deltaPos[j], 1);
            glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"modelMat"),1,GL_FALSE,&model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"projMat"),1,GL_FALSE,&proj[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"viewMat"),1,GL_FALSE,&viewMat[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(terrain_shader,"worldNormalMat"),1,GL_FALSE,&itctm[0][0]);
            glBindVertexArray(terrain_vao[0]);
            glDrawArrays(GL_TRIANGLES,0,terrain_size);
            glUseProgram(0);
        }
    }*/

}

void Realtime::initializeScene(){
    //initialize camera

    vec4 birdPos = vec4(-150,20,-150,1);

    vec4 pos = vec4(-162,30,-160,1);
    vec4 look = normalize(birdPos-pos);
    vec4 up = vec4(0,1,0,0);
    float heightAngle = glm::radians(30.f);

    camera = Camera(pos,look,up,heightAngle,0,0,size().height(),size().width(),10,200.f);

    //initialize light sources
    SceneLightData light1;
    light1.id = 0;
    light1.type = LightType::LIGHT_DIRECTIONAL;
    light1.dir = vec4(1,-1,1,0);
    light1.color = vec4(1.f);

    lights.push_back(light1);
    //light camera
    float near_plane = 10.f, far_plane = 100.f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    glm::vec3 lightPos = vec3(30,30,15);
    glm::vec3 at = glm::vec3(0, 15, 15);

    glm::mat4 lightView = glm::lookAt(lightPos,at,vec3(up));


    parseGlobal();
    parseLights();
    //terrain
    TerrainGenerator m_terrain;
    std::vector<GLfloat> verts = m_terrain.generateTerrain();
    invertX(verts);
    invertY(verts);

    glGenBuffers(1,&terrain_vbo);
    glGenVertexArrays(1,&terrain_vao);
    glBindVertexArray(terrain_vao);

    glBindBuffer(GL_ARRAY_BUFFER, terrain_vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), &verts[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                             nullptr);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat),
                             reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    terrain_size = verts.size();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);






}

void invertX(std::vector<float> &verts){
    int n = verts.size();
    for(int i=0;i<n;i+=9){
        verts.push_back(-verts[i+0]);
        verts.push_back(verts[i+1]);
        verts.push_back(verts[i+2]);
        verts.push_back(-verts[i+3]);
        verts.push_back(verts[i+4]);
        verts.push_back(verts[i+5]);
        verts.push_back(verts[i+6]);
        verts.push_back(verts[i+7]);
        verts.push_back(verts[i+8]);
    }
}
void invertY(std::vector<float> &verts){
    int n = verts.size();
    for(int i=0;i<n;i+=9){
        verts.push_back(verts[i+0]);
        verts.push_back(-verts[i+1]);
        verts.push_back(verts[i+2]);
        verts.push_back(verts[i+3]);
        verts.push_back(-verts[i+4]);
        verts.push_back(verts[i+5]);
        verts.push_back(verts[i+6]);
        verts.push_back(verts[i+7]);
        verts.push_back(verts[i+8]);
    }
}

