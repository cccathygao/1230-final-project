#include "realtime.h"
#include "settings.h"
#include <iostream>

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
        currShape.updateParams(settings.shapeParameter1,settings.shapeParameter2);
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
