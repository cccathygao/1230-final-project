#include "realtime.h"

#include "utils/shaderloader.h"
#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/constants.hpp"

using namespace glm;

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this

}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
    glDeleteBuffers(1,&shape_vbos[0]);
    glDeleteVertexArrays(1,&shape_vaos[0]);
    glDeleteProgram(m_phong_shader);
    glDeleteFramebuffers(1,&m_fbo);
    glDeleteTextures(1,&depth_texture);
    glDeleteTextures(1,&color_texture);
    //glDeleteRenderbuffers(1,&m_fbo_renderbuffer);
    glDeleteProgram(m_fbo_shader);

    // shadow mapping
    // depth fbo
    glDeleteTextures(1, &m_depth_texture);
    glDeleteFramebuffers(1, &m_depth_fbo);

    // depth shader
    glDeleteProgram(m_depth_shader);

    // bird vao and vbo
    GLuint bird_vbo_id = m_bird_vbo.getId();
    GLuint bird_vao_id = m_bird_vao.getId();

    glDeleteBuffers(1, &bird_vbo_id);
    glDeleteVertexArrays(1, &bird_vao_id);

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    //glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    default_fbo = 3;
    screen_width = size().width() * m_devicePixelRatio;
    screen_height = size().height() * m_devicePixelRatio;
    fbo_width = screen_width;
    fbo_height = screen_height;

    initialized = true;
    m_phong_shader = ShaderLoader::createShaderProgram(":/resources/shaders/phong.vert",":/resources/shaders/phong.frag");
    m_fbo_shader = ShaderLoader::createShaderProgram(":/resources/shaders/fbo.vert", ":/resources/shaders/fbo.frag");
    bird_shader = ShaderLoader::createShaderProgram(":/resources/shaders/bird.vert", ":/resources/shaders/bird.frag");
    m_depth_shader = ShaderLoader::createShaderProgram(":/resources/shaders/depth.vert", ":/resources/shaders/depth.frag");
    setupShapesVBO();
    updateShapesVBO();
    setFullscreenquad();
    makeFBO();
    makeDepthFBO();

    // generate VAO and VBO
    m_bird_vao.initialize();
    m_bird_vbo.initialize();

    loadOBJ();
    initializeScene();
    bird_ctm = mat4(1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,15,15,1);
    normal_ctm = inverse(transpose(bird_ctm));
    //

}

void Realtime::paintGL() {
    // shadow mapping, paint depth texture
    glBindFramebuffer(GL_FRAMEBUFFER, m_depth_fbo);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderDepthMap();

    glClearColor(0.8,1,1,1);
    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int vp[4] = {0,0,0,0};
    glGetIntegerv(GL_VIEWPORT, &vp[0]);
    glViewport(0,0,fbo_width,fbo_height);
    paintLand();
    paintBird();

    glBindFramebuffer(GL_FRAMEBUFFER,default_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(vp[0],vp[1],vp[2],vp[3]);
    paintTexture(color_texture);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    screen_width = size().width() * m_devicePixelRatio;
    screen_height = size().height() * m_devicePixelRatio;
    fbo_width = screen_width;
    fbo_height = screen_height;
    // Students: anything requiring OpenGL calls when the program starts should be done here
    glDeleteTextures(1,&depth_texture);
    glDeleteTextures(1,&color_texture);
    //glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1,&m_fbo);
    makeFBO();
    camera.setAspectRatio(size().width(),size().height());
    camera.modifyProj();
    update();
}

void Realtime::sceneChanged() {
    /*
    sceneLoaded = true;
    bool success = SceneParser::parse(settings.sceneFilePath, metaData);
    if (!success) {
            std::cerr << "Error loading scene" << std::endl;
            exit(1);
            return ;
    }
    setupCamera();
    update(); // asks for a PaintGL() call to occur*/
}

void Realtime::settingsChanged() {
    /*
    if(settingChangeType()==SettingType::CAMERA_ONLY){
        if(sceneLoaded){
            camera.setNear(settings.nearPlane);
            camera.setFar(settings.farPlane);
            camera.modifyProj();
        }
    }
    else if(settingChangeType()==SettingType::CAMERA_AND_SHAPE){
        makeCurrent();
        updateShapesVBO();
        if(sceneLoaded){
            camera.setNear(settings.nearPlane);
            camera.setFar(settings.farPlane);
            camera.modifyProj();
        }
        doneCurrent();
    }
    else{
        makeCurrent();
        updateShapesVBO();
        doneCurrent();
    }*/
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        rotateCamera(deltaX, deltaY);
        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();
//    mat4 trans = mat4(1,0,0,0,
//                      0,1,0,0,
//                      0,0,1,0,
//                      0,0,0.01,1);
//    bird_ctm = trans*bird_ctm;
//    normal_ctm = inverse(transpose(bird_ctm));
//    vec4 pos = camera.getPos()+vec4(0,0,0.01,0);
//    camera.setPos(pos);
//    camera.modifyView();
//    parseGlobal();
    std::cout<<"time"<<std::endl;
    // Use deltaTime and m_keyMap here to move around
    moveCamera(deltaTime);
    update(); // asks for a PaintGL() call to occur
}


// shadow mapping
void Realtime::makeDepthFBO(){
    // generate and bind an empty texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    m_depth_texture = depthMap;

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, m_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindTexture(GL_TEXTURE_2D, 0);

    // generate and bind depth FBO
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    m_depth_fbo = depthMapFBO;

    glBindFramebuffer(GL_FRAMEBUFFER, m_depth_fbo);

    // add texture as a depth attachment to depth FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Realtime::lightSpaceTransform(SceneLightData light){
    // TODO: how to do choose near and far
    float near_plane = 0.1f, far_plane = 100.f;

    // pos for dir light: 3 from the center of scene
    glm::vec3 lightPos = -glm::normalize(glm::vec3(light.dir)) * 30.f;

    // orthographic projection for directional light
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    // light source position looking at scene's center
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (light.dir == glm::vec4(0.f, -1.f, 0.f, 0.f) || light.dir == glm::vec4(0.f, 1.f, 0.f, 0.f)){
        up = glm::vec3(1.0f, 1.0f, 0.0f);
    }
    glm::mat4 lightView = glm::lookAt(lightPos,
                                      glm::vec3( 0.0f, 0.0f,  0.0f),
                                      up);

    // world space to clip space
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    m_lightSpaceMatrix = lightSpaceMatrix;
}

// render depth map to depth fbo
// calls all relevant drawing functions
// & sets the corresponding model matrices where necessary.
void Realtime::renderDepthMap(){
    glUseProgram(m_depth_shader);

//    std::cout << "mat_light_RenderDepth" << std::endl;
//    std::cout << glm::to_string(m_lightSpaceMatrix) << std::endl;

    // set uniforms
    // light space transformation matrix
    GLint lightSpaceMatrixLocation = glGetUniformLocation(m_depth_shader, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &m_lightSpaceMatrix[0][0]);

    // model matrix
    GLint modelMatrixLocation = glGetUniformLocation(m_depth_shader, "mat_model");
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &bird_ctm[0][0]);

    // draw
    m_bird_vao.draw(m_buffer.size(), -1, -1);

    glUseProgram(0);
}
