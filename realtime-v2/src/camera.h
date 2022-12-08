#pragma once

#include <glm/glm.hpp>
#include <cmath>
// A class representing a virtual camera.

class Camera {
public:
    Camera(glm::vec4 pos,glm::vec4 look,glm::vec4 up,float heightAngle,float aperture,float focalLength, int imageH, int imageW,
           float near, float far);
    Camera();
    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    glm::mat4 &getViewMatrix();
    glm::mat4 &getInverseV();
    // Returns the aspect ratio of the camera.
    float getAspectRatio();

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle();

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength();

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture();


    float getViewPlaneHeight() ;
    float getViewPlaneWidth() ;
    float getK() ;
    glm::vec4 getPos();
    glm::mat4 calculateViewMatrix(glm::vec3 pos, glm::vec3 look, glm::vec3 up);
    glm::mat4 calculateProjMatrix(float near, float far, float heightAngle, float widthAngle);
    glm::mat4& getProjMatrix();
    float getWidthAngle();
    glm::vec4 getLook();
    glm::vec4 getUp();
    void modifyProj();
    void modifyView();
    void setAspectRatio(int w, int h);
    void setFar(float f);
    void setNear(float n);
    void setPos(glm::vec4 pos);
    void setLook(glm::vec4 look);
    void setUp(glm::vec4 up);
private:
    glm::mat4 viewMatrix;
    glm::mat4 inverseView;
    glm::mat4 projMatrix;
    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;
    float heightAngle;
    float aperture;
    float focalLength;
    float aspectRatio;
    float k = 1.f;
    float far;
    float near;
};
