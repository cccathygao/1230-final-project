#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "utils/sceneparser.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>
#include <data/vbo.h>
#include <data/vao.h>
#include "camera/camera.h"
#include "shape/cube.h"
#include "shape/cone.h"
#include "shape/cylinder.h"
#include "shape/sphere.h"
#include "QtGui/qimage.h"
#include "utils/tiny_obj_loader.h"
#include "utils/meshloader.h"

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void rebuildMatrices();     // update camera matrices based on mouse events
    void makeFBO();             // make FBO
    void paintScene();          // paint scene
    void paintTexture();  // paint texture
    void setViewMatrix(glm::vec3 look, glm::vec3 up, glm::vec3 pos);
    void initFullscreenQuad();
    // shadow mapping
    void makeDepthFBO();
    void lightSpaceTransform(SceneLightData light);
    void renderDepthMap();
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void insertVec2(std::vector<float> &data, glm::vec2 v);
    void load(const std::string &path);

   // shadow mapping
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    glm::mat4 m_lightSpaceMatrix = glm::mat4(1);
    GLuint m_depth_fbo;
    GLuint m_depth_texture;

    // obj loader for eagle
    VAO m_eagle_vao = VAO();
    VBO m_eagle_vbo = VBO();
    std::vector<float> m_eagleData = {};

    // texture mapping
    GLuint loadImageFromFile(const QString &file);

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;

    bool m_isInitialized = false;

    RenderData m_metaData;
    Camera m_camera;

    glm::mat4 m_model = glm::mat4(1);
    glm::mat4 m_view  = glm::mat4(1);
    glm::mat4 m_proj  = glm::mat4(1);
    glm::mat3 m_model_inv_trans = glm::mat3(1);

    float m_angleX;
    float m_angleY;

    int m_numLights = 0;
    std::vector<int> m_lightTypes;
    std::vector<glm::vec4> m_lightColors;
    std::vector<glm::vec4> m_lightDirs;
    std::vector<glm::vec4> m_lightPositions;
    std::vector<glm::vec3> m_lightFunctions;
    std::vector<float> m_lightPenumbras;
    std::vector<float> m_lightAngles;

    float m_ka;
    float m_kd;
    float m_ks;

    SceneMaterial m_material;

    std::shared_ptr<Shape> m_cube = std::make_shared<Cube>();
    std::shared_ptr<Shape> m_sphere = std::make_shared<Sphere>();
    std::shared_ptr<Shape> m_cylinder = std::make_shared<Cylinder>();
    std::shared_ptr<Shape> m_cone = std::make_shared<Cone>();

    // tessellation params
    int m_currParam1;
    int m_currParam2;

    // shaders
    GLuint m_phong_shader;      // draw image to FBO
    GLuint m_fullscreen_shader;    // draw from FBO to screen
    GLuint m_depth_shader;      // shadow mapping

    // VAO & VBO
    VAO m_cube_vao = VAO();
    VAO m_sphere_vao = VAO();
    VAO m_cylinder_vao = VAO();
    VAO m_cone_vao = VAO();

    VBO m_cube_vbo = VBO();
    VBO m_sphere_vbo = VBO();
    VBO m_cylinder_vbo = VBO();
    VBO m_cone_vbo = VBO();

    std::vector<float> m_cubeData;
    std::vector<float> m_sphereData;
    std::vector<float> m_cylinderData;
    std::vector<float> m_coneData;

    // fullscreen data
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;

    // FBO
    GLuint m_fbo;
    GLuint m_defaultFBO = 3;
    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;
    int m_fbo_width;
    int m_fbo_height;

    // post-processing
    bool m_invert_filter_on = false;
    bool m_grayscale_filter_on = false;
    bool m_blur_filter_on = false;
    bool m_sharpen_filter_on = false;

    // texture mapping
    bool m_texture_map_on = false;
    std::unordered_map<std::string, GLuint> m_imagemap;

    // shadow mapping eagle obj
    bool m_eagle_shadow_on = false;

    // extra credit 1: adaptive level of details
    VAO m_cube_vao_adaptive = VAO();
    VAO m_sphere_vao_adaptive = VAO();
    VAO m_cylinder_vao_adaptive = VAO();
    VAO m_cone_vao_adaptive = VAO();

    VBO m_cube_vbo_adaptive = VBO();
    VBO m_sphere_vbo_adaptive = VBO();
    VBO m_cylinder_vbo_adaptive = VBO();
    VBO m_cone_vbo_adaptive = VBO();

    std::vector<float> m_cubeData_adaptive;
    std::vector<float> m_sphereData_adaptive;
    std::vector<float> m_cylinderData_adaptive;
    std::vector<float> m_coneData_adaptive;

    int m_numShapes;
};
