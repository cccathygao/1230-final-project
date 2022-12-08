#include "realtime.h"

#include "utils/shaderloader.h"
#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"

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
    glDeleteProgram(m_shader);
    glDeleteFramebuffers(1,&m_fbo);
    glDeleteTextures(1,&depth_texture);
    glDeleteTextures(1,&color_texture);
    //glDeleteRenderbuffers(1,&m_fbo_renderbuffer);
    glDeleteProgram(m_fbo_shader);
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
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    default_fbo = 2;
    screen_width = size().width() * m_devicePixelRatio;
    screen_height = size().height() * m_devicePixelRatio;
    fbo_width = screen_width;
    fbo_height = screen_height;

    initialized = true;
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert",":/resources/shaders/default.frag");
    m_fbo_shader = ShaderLoader::createShaderProgram(":/resources/shaders/fbo.vert", ":/resources/shaders/fbo.frag");
    setupShapesVBO();
    updateShapesVBO();
    setFullscreenquad();
    makeFBO();
}

void Realtime::paintGL() {

    glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int vp[4] = {0,0,0,0};
    glGetIntegerv(GL_VIEWPORT, &vp[0]);
    glViewport(0,0,fbo_width,fbo_height);
    paintScene();
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
    sceneLoaded = true;
    bool success = SceneParser::parse(settings.sceneFilePath, metaData);
    if (!success) {
            std::cerr << "Error loading scene" << std::endl;
            exit(1);
            return ;
    }
    setupCamera();
    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
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
    }
    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
    if(m_keyMap[Qt::Key_Q]){
        focalPoint += 0.1;
    }
    if(m_keyMap[Qt::Key_Z]){
        focalPoint -= 0.1;
    }
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

    // Use deltaTime and m_keyMap here to move around
    moveCamera(deltaTime);
    update(); // asks for a PaintGL() call to occur
}
