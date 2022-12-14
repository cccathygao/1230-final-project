#pragma once

#include "utils/scenedata.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:
    void init(SceneCameraData data, float aspectRatio);

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    // Returns the view matrix for the current camera settings.
    glm::mat4 getViewMatrix() const;

    // Returns the projection matrix for the current camera settings.
    glm::mat4 getProjectionMatrix() const;

    // Returns camera position.
    glm::vec4 getCameraPos() const;

    // Returns look.
    glm::vec3 getCameraLook() const;

    // Returns up.
    glm::vec3 getCameraUp() const;

    void setViewMatrix(glm::vec3 look, glm::vec3 up, glm::vec3 pos);

    void setProjectionMatrix();
    void setNear(float near);
    void setFar(float far);
    void setAspectRatio(float aspectRatio);

private:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_scaleMatrix;
    glm::mat4 m_unhingeMatrix;
    glm::mat4 m_projectionMatrix;

    glm::vec4 m_pos;

    glm::vec3 m_look;
    glm::vec3 m_up;

    float m_aspectRatio;
    float m_heightAngle;

    float m_near = 0.1;
    float m_far = 100.f;
};
