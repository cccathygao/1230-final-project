#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "utils/sceneparser.h"
#include "glm/gtc/constants.hpp"
#include "src/shaderloader.h"
#include <string>
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "utils/meshloader.h"
#include "utils/tiny_obj_loader.h"

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
    // Clean up you VBO and VAO memory here

    GLuint cube_vbo_id = m_cube_vbo.getId();
    GLuint cube_vao_id = m_cube_vao.getId();
    GLuint sphere_vbo_id = m_sphere_vbo.getId();
    GLuint sphere_vao_id = m_sphere_vao.getId();
    GLuint cylinder_vbo_id = m_cylinder_vbo.getId();
    GLuint cylinder_vao_id = m_cylinder_vao.getId();
    GLuint cone_vbo_id = m_cone_vbo.getId();
    GLuint cone_vao_id = m_cone_vao.getId();

    glDeleteBuffers(1, &cube_vbo_id);
    glDeleteVertexArrays(1, &cube_vao_id);
    glDeleteBuffers(1, &sphere_vbo_id);
    glDeleteVertexArrays(1, &sphere_vao_id);
    glDeleteBuffers(1, &cylinder_vbo_id);
    glDeleteVertexArrays(1, &cylinder_vao_id);
    glDeleteBuffers(1, &cone_vbo_id);
    glDeleteVertexArrays(1, &cone_vao_id);

    // fullscreen
    glDeleteBuffers(1, &m_fullscreen_vbo);
    glDeleteVertexArrays(1, &m_fullscreen_vao);

    // fbo
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    // depth fbo
    glDeleteTextures(1, &m_depth_texture);
    glDeleteFramebuffers(1, &m_depth_fbo);

    glDeleteProgram(m_phong_shader);
    glDeleteProgram(m_fullscreen_shader);
    glDeleteProgram(m_depth_shader);
    this->doneCurrent();
}

void Realtime::initializeGL() {

    std::cout << "initializeGL" << std::endl;

    m_devicePixelRatio = this->devicePixelRatio();

    m_fbo_width = size().width() * m_devicePixelRatio;
    m_fbo_height = size().height() * m_devicePixelRatio;

    std::cout << "width" << m_fbo_width << std::endl;
    std::cout << "height" << m_fbo_height << std::endl;

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
    // Set the clear color here
    glClearColor(0, 0, 0, 1);

    // load shader
    m_phong_shader = ShaderLoader::createShaderProgram(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");
    m_fullscreen_shader = ShaderLoader::createShaderProgram(":/resources/shaders/fullscreen.vert", ":/resources/shaders/fullscreen.frag");
    m_depth_shader = ShaderLoader::createShaderProgram(":/resources/shaders/depth.vert", ":/resources/shaders/depth.frag");
    std::cout<<"shader loaded"<<std::endl;

    // obj loader for eagle
    m_eagle_vao.initialize();
    m_eagle_vbo.initialize();
    m_eagle_vao.setAttributes(m_eagle_vbo.getId());

    // initialize VBO, VAO
    m_cube_vao.initialize();
    m_sphere_vao.initialize();
    m_cylinder_vao.initialize();
    m_cone_vao.initialize();

    m_cube_vbo.initialize();
    m_sphere_vbo.initialize();
    m_cylinder_vbo.initialize();
    m_cone_vbo.initialize();

    // set VAO attributes (only need to do it once)
    m_cube_vao.setAttributes(m_cube_vbo.getId());
    m_sphere_vao.setAttributes(m_sphere_vbo.getId());
    m_cylinder_vao.setAttributes(m_cylinder_vbo.getId());
    m_cone_vao.setAttributes(m_cone_vbo.getId());

    // generate initial vertex data
    m_currParam1 = settings.shapeParameter1;
    m_currParam2 = settings.shapeParameter2;

    m_cube->updateParams(m_currParam1, m_currParam2);
    m_sphere->updateParams(m_currParam1, m_currParam2);
    m_cylinder->updateParams(m_currParam1, m_currParam2);
    m_cone->updateParams(m_currParam1, m_currParam2);

    m_cubeData = m_cube->getVertexData();
    m_sphereData = m_sphere->getVertexData();
    m_cylinderData = m_cylinder->getVertexData();
    m_coneData = m_cone->getVertexData();

    // set VBO data
    m_cube_vbo.setData(m_cubeData);
    m_sphere_vbo.setData(m_sphereData);
    m_cylinder_vbo.setData(m_cylinderData);
    m_cone_vbo.setData(m_coneData);

    // initialize fullscreen quad
    initFullscreenQuad();

    // depth FBO
    // debug
    makeDepthFBO();

    // FBO
    makeFBO();

    m_isInitialized = true;
}

void Realtime::paintGL() {
    // std::cout << "paintGL" << std::endl;

//     Students: anything requiring OpenGL calls every frame should be done here

    // debug
    glBindFramebuffer(GL_FRAMEBUFFER, m_depth_fbo);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderDepthMap();


    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_fbo_width, m_fbo_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintScene();

//    paintTexture();
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    m_camera.setAspectRatio(static_cast<float>(w) / static_cast<float>(h));
    m_camera.setProjectionMatrix();
    m_proj = m_camera.getProjectionMatrix();

    // regenerate FBO
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    m_fbo_width = size().width() * m_devicePixelRatio;
    m_fbo_height = size().height() * m_devicePixelRatio;

    makeFBO();
}

void Realtime::sceneChanged() {

    std::cout << "scene changed" << std::endl;

    if (!m_isInitialized){
        return;
    }

    bool success = SceneParser::parse(settings.sceneFilePath, m_metaData);

    if (!success) {
        std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
        return;
    }

    float aspectRatio = static_cast<float>(size().width()) / static_cast<float>(size().height());
    m_camera.init(m_metaData.cameraData, aspectRatio);

    m_angleX = 0.f;
    m_angleY = 0.f;

    // update lights
    m_numLights = 0;
    m_lightTypes.clear();
    m_lightColors.clear();
    m_lightDirs.clear();
    m_lightPositions.clear();
    m_lightFunctions.clear();
    m_lightPenumbras.clear();
    m_lightAngles.clear();

    m_numLights = m_metaData.lights.size();

    for(SceneLightData light : m_metaData.lights){
        if (light.type == LightType::LIGHT_DIRECTIONAL){
            m_lightTypes.push_back(0);
            lightSpaceTransform(light);

        } else if (light.type == LightType::LIGHT_POINT){
            m_lightTypes.push_back(1);
        } else if (light.type == LightType::LIGHT_SPOT){
            m_lightTypes.push_back(2);
        }
        m_lightColors.push_back(light.color);
        m_lightDirs.push_back(light.dir);
        m_lightPositions.push_back(light.pos);
        m_lightFunctions.push_back(light.function);
        m_lightPenumbras.push_back(light.penumbra);
        m_lightAngles.push_back(light.angle);
    }

    // update material parameters
    m_ka = m_metaData.globalData.ka;
    m_ks = m_metaData.globalData.ks;
    m_kd = m_metaData.globalData.kd;

    // update camera matrices
    m_camera.setFar(settings.farPlane);
    m_camera.setNear(settings.nearPlane);
    m_camera.setProjectionMatrix();
    m_view = m_camera.getViewMatrix();
    m_proj = m_camera.getProjectionMatrix();

    // texture mapping
    // clear imagemap & textures
    for(auto& item : m_imagemap){
        GLuint texture_id = item.second;
        glDeleteTextures(1, &texture_id);
        m_imagemap.erase(item.first);
    }

    // load new textures
    for (RenderShapeData const &shapeData : m_metaData.shapes){
        if (shapeData.primitive.material.textureMap.isUsed){
            std::string filename = shapeData.primitive.material.textureMap.filename;
            if(m_imagemap.count(filename) == 0){
                m_imagemap[filename] = loadImageFromFile(QString::fromStdString(filename));
            }
        }
    }

    // number of objects in the scene
    m_numShapes = m_metaData.shapes.size();

    // obj loader eagle
//    Loader loader;
//    loader.load(settings.sceneFilePath);
//    m_eagleData = loader.getVertexData();
    const std::string &s = settings.sceneFilePath;
    load(s);

    this->makeCurrent();

    m_eagle_vbo.setData(m_eagleData);

    // set VBO data
    m_cube_vbo_adaptive.setData(m_cubeData_adaptive);
    m_sphere_vbo_adaptive.setData(m_sphereData_adaptive);
    m_cylinder_vbo_adaptive.setData(m_cylinderData_adaptive);
    m_cone_vbo_adaptive.setData(m_coneData_adaptive);


    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {

    std::cout << "settings changed" << std::endl;

    if (!m_isInitialized){
        return;
    }

    // update camera matrices
    m_camera.setFar(settings.farPlane);
    m_camera.setNear(settings.nearPlane);
    m_camera.setProjectionMatrix();
    m_view = m_camera.getViewMatrix();
    m_proj = m_camera.getProjectionMatrix();

    // update params and vertex data
    m_currParam1 = settings.shapeParameter1;
    m_currParam2 = settings.shapeParameter2;

    m_cube->updateParams(m_currParam1, m_currParam2);
    m_sphere->updateParams(m_currParam1, m_currParam2);
    m_cylinder->updateParams(m_currParam1, m_currParam2);
    m_cone->updateParams(m_currParam1, m_currParam2);

    // get vertex data
    m_cubeData = m_cube->getVertexData();
    m_sphereData = m_sphere->getVertexData();
    m_cylinderData = m_cylinder->getVertexData();
    m_coneData = m_cone->getVertexData();

    this->makeCurrent();

    // set data in VBO
    m_cube_vbo.setData(m_cubeData);
    m_sphere_vbo.setData(m_sphereData);
    m_cylinder_vbo.setData(m_cylinderData);
    m_cone_vbo.setData(m_coneData);

    // texture mapping
    m_texture_map_on = settings.extraCredit3;

    // shadow mapping for eagle
    m_eagle_shadow_on = settings.extraCredit4;

    m_invert_filter_on = settings.perPixelFilter;
    m_blur_filter_on = settings.kernelBasedFilter;

    update(); // asks for a PaintGL() call to occur
}

void Realtime::makeFBO(){
    // Generate and bind an empty texture, set its min/mag filter interpolation, then unbind
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    m_fbo_texture = texture_id;

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Generate and bind a renderbuffer of the right size, set its format, then unbind
    GLuint renderbuffer_id;
    glGenRenderbuffers(1, &renderbuffer_id);
    m_fbo_renderbuffer = renderbuffer_id;

    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Generate and bind an FBO
    GLuint framebuffer_id;
    glGenFramebuffers(1, &framebuffer_id);
    m_fbo = framebuffer_id;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Add our texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    // Unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

void Realtime::paintScene(){

    std::cout << "m_depth_texture: " << m_depth_texture << std::endl;

    // Clear screen color and depth before painting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate the shader program by calling glUseProgram with `m_phong_shader`
    glUseProgram(m_phong_shader);

    // TODO: shadow mapping
    // depth map
    GLint depthTextureLocation = glGetUniformLocation(m_phong_shader, "depthMap");
    glUniform1i(depthTextureLocation, 1);

    // light space transformation matrix
    GLint lightSpaceMatrixLocation = glGetUniformLocation(m_phong_shader, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &m_lightSpaceMatrix[0][0]);


    // number of lights
    GLint numLightsLocation = glGetUniformLocation(m_phong_shader, "numLights");
    glUniform1i(numLightsLocation, m_numLights);

    // light data
    for (int j = 0; j < m_numLights; j++){
        GLint lightTypesLocation = glGetUniformLocation(m_phong_shader, ("lightTypes[" + std::to_string(j) + "]").c_str());
        glUniform1i(lightTypesLocation, m_lightTypes[j]);

        GLint lightColorsLocation = glGetUniformLocation(m_phong_shader, ("lightColors[" + std::to_string(j) + "]").c_str());
        glUniform4f(lightColorsLocation, m_lightColors[j][0], m_lightColors[j][1], m_lightColors[j][2], m_lightColors[j][3]);

        GLint lightDirsLocation = glGetUniformLocation(m_phong_shader, ("lightDirs[" + std::to_string(j) + "]").c_str());
        glUniform4f(lightDirsLocation, m_lightDirs[j][0], m_lightDirs[j][1], m_lightDirs[j][2], m_lightDirs[j][3]);

        GLint lightPositionsLocation = glGetUniformLocation(m_phong_shader, ("lightPositions[" + std::to_string(j) + "]").c_str());
        glUniform4f(lightPositionsLocation, m_lightPositions[j][0], m_lightPositions[j][1], m_lightPositions[j][2], m_lightPositions[j][3]);

        GLint lightFunctionsLocation = glGetUniformLocation(m_phong_shader, ("lightFunctions[" + std::to_string(j) + "]").c_str());
        glUniform3f(lightFunctionsLocation, m_lightFunctions[j][0], m_lightFunctions[j][1], m_lightFunctions[j][2]);

        GLint lightPenumbrasLocation = glGetUniformLocation(m_phong_shader, ("lightPenumbras[" + std::to_string(j) + "]").c_str());
        glUniform1f(lightPenumbrasLocation, m_lightPenumbras[j]);

        GLint lightAnglesLocation = glGetUniformLocation(m_phong_shader, ("lightAngles[" + std::to_string(j) + "]").c_str());
        glUniform1f(lightAnglesLocation, m_lightAngles[j]);
    }

    // camera loc
    GLint cameraLocation = glGetUniformLocation(m_phong_shader, "camera_pos");
    glm::vec4 cameraPos = m_camera.getCameraPos();
    glUniform4f(cameraLocation, cameraPos[0], cameraPos[1], cameraPos[2], cameraPos[3]);

    // ka, kd, ks
    GLint kaLocation = glGetUniformLocation(m_phong_shader, "k_a");
    glUniform1f(kaLocation, m_ka);

    GLint kdLocation = glGetUniformLocation(m_phong_shader, "k_d");
    glUniform1f(kdLocation, m_kd);

    GLint ksLocation = glGetUniformLocation(m_phong_shader, "k_s");
    glUniform1f(ksLocation, m_ks);

    // texture mapping
    GLuint textureMapOnLocation = glGetUniformLocation(m_phong_shader, "texture_map_on");
    glUniform1i(textureMapOnLocation, m_texture_map_on);

    for (RenderShapeData const &shapeData : m_metaData.shapes){
        m_model = shapeData.ctm;
        m_model_inv_trans = glm::inverse(glm::transpose(glm::mat3(shapeData.ctm)));
        m_material = shapeData.primitive.material;

        // set uniform
        // matrices
        GLint modelMatrixLocation = glGetUniformLocation(m_phong_shader, "mat_model");
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &m_model[0][0]);

        GLint modelInvTransMatrixLocation = glGetUniformLocation(m_phong_shader, "mat_model_inv_trans");
        glUniformMatrix3fv(modelInvTransMatrixLocation, 1, GL_FALSE, &m_model_inv_trans[0][0]);

        GLint viewMatrixLocation = glGetUniformLocation(m_phong_shader, "mat_view");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &m_view[0][0]);

        GLint projMatrixLocation = glGetUniformLocation(m_phong_shader, "mat_proj");
        glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &m_proj[0][0]);

        // cAmbient, cDiffuse, cSpecular
        GLint cAmbientLocation = glGetUniformLocation(m_phong_shader, "cAmbient");
        glUniform4f(cAmbientLocation, m_material.cAmbient[0], m_material.cAmbient[1], m_material.cAmbient[2],m_material.cAmbient[3]);

        GLint cDiffuseLocation = glGetUniformLocation(m_phong_shader, "cDiffuse");
        glUniform4f(cDiffuseLocation, m_material.cDiffuse[0], m_material.cDiffuse[1], m_material.cDiffuse[2], m_material.cDiffuse[3]);

        GLint cSpecularLocation = glGetUniformLocation(m_phong_shader, "cSpecular");
        glUniform4f(cSpecularLocation, m_material.cSpecular[0], m_material.cSpecular[1], m_material.cSpecular[2], m_material.cSpecular[3]);

        // shininess
        GLint shininessLocation = glGetUniformLocation(m_phong_shader, "shininess");
        glUniform1f(shininessLocation, m_material.shininess);

        // blend
        GLint blendLocation = glGetUniformLocation(m_phong_shader, "blend");
        glUniform1f(blendLocation, m_material.blend);

        // texture mapping
        GLuint texture_id = -1;
        if(m_material.textureMap.isUsed){
            std::string filename = m_material.textureMap.filename;

            // get texture id from imagemap
            texture_id = m_imagemap[filename];

            // Set the active texture slot to texture slot 0
            glActiveTexture(GL_TEXTURE0);

            // set uniform: textureImg
            GLint textureImgLocation = glGetUniformLocation(m_phong_shader, "textureImg");
            glUniform1i(textureImgLocation, 0);

            // repeatU, repeatV
            GLint repeatULocation = glGetUniformLocation(m_phong_shader, "repeatU");
            glUniform1f(repeatULocation, m_material.textureMap.repeatU);

            GLint repeatVLocation = glGetUniformLocation(m_phong_shader, "repeatV");
            glUniform1f(repeatVLocation, m_material.textureMap.repeatV);
        }

        // eagle shadow mapping
        if (m_eagle_shadow_on) {
            m_eagle_vao.draw(m_eagleData.size(), texture_id, m_depth_texture);
        } else {
            // draw
            switch(shapeData.primitive.type){
            case PrimitiveType::PRIMITIVE_CUBE:
                m_cube_vao.draw(m_cubeData.size(), texture_id, m_depth_texture);
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_sphere_vao.draw(m_sphereData.size(), texture_id, m_depth_texture);
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_cylinder_vao.draw(m_cylinderData.size(), texture_id, m_depth_texture);
                break;
            case PrimitiveType::PRIMITIVE_CONE:

                break;
            default:
                break;
            }
        }
    }

    // deactivate the shader program by passing 0 into glUseProgram
    glUseProgram(0);
}

void Realtime::paintTexture(){
    std::cout << "paint Texture" << std::endl;

    glUseProgram(m_fullscreen_shader);

    // set uniforms
    // depthMap texture
    // debug
//    GLint depthTextureLocation = glGetUniformLocation(m_fullscreen_shader, "depthMap");
//    glUniform1i(depthTextureLocation, 0);

    // colorBuffer texture
    GLint colorTextureLocation = glGetUniformLocation(m_fullscreen_shader, "colorBuffer");
    glUniform1i(colorTextureLocation, 0);

    // post-processing switch
    GLuint invertFilterOnLocation = glGetUniformLocation(m_fullscreen_shader, "invert_filter_on");
    glUniform1i(invertFilterOnLocation, m_invert_filter_on);

    GLuint blurFilterOnLocation = glGetUniformLocation(m_fullscreen_shader, "blur_filter_on");
    glUniform1i(blurFilterOnLocation, m_blur_filter_on);

    GLuint grayscaleFilterOnLocation = glGetUniformLocation(m_fullscreen_shader, "grayscale_filter_on");
    glUniform1i(grayscaleFilterOnLocation, m_grayscale_filter_on);

    GLuint sharpenFilterOnLocation = glGetUniformLocation(m_fullscreen_shader, "sharpen_filter_on");
    glUniform1i(sharpenFilterOnLocation, m_sharpen_filter_on);

    GLuint widthLocation = glGetUniformLocation(m_fullscreen_shader, "width");
    glUniform1i(widthLocation, m_fbo_width);

    GLuint heightLocation = glGetUniformLocation(m_fullscreen_shader, "height");
    glUniform1i(heightLocation, m_fbo_height);

    // draw to fullscreen quad
    glBindVertexArray(m_fullscreen_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    // debug
//    glBindTexture(GL_TEXTURE_2D, m_depth_texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    glUseProgram(0);
}

void Realtime::initFullscreenQuad(){
    std::vector<GLfloat> fullscreen_quad_data =
    {
        -1.0f,  1.0f, 0.0f,
         0.f, 1.f,
        -1.0f, -1.0f, 0.0f,
         0.f, 0.f,
         1.0f, -1.0f, 0.0f,
         1.f, 0.f,
         1.0f,  1.0f, 0.0f,
         1.f, 1.f,
        -1.0f,  1.0f, 0.0f,
         0.f, 1.f,
         1.0f, -1.0f, 0.0f,
         1.f, 0.f
    };

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    // attribute 0: pos, 1: uv
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint Realtime::loadImageFromFile(const QString &file) {
    QImage myImage;
    if (!myImage.load(file)) {
        std::cout<<"Failed to load in image"<<std::endl;
        return -1;
    }
    myImage = myImage.convertToFormat(QImage::Format_RGBX8888).mirrored();

    // Generate texture
    GLuint texture_id;
    glGenTextures(1, &texture_id);

    // Set the active texture slot to texture slot 0
    glActiveTexture(GL_TEXTURE0);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Load image into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myImage.width(), myImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myImage.bits());

    // Set min and mag filters' interpolation mode to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture_id;
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
        m_angleX = deltaX / (float)width();
        m_angleY = deltaY / (float)height();

        rebuildMatrices();

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::rebuildMatrices() {

    // x: rotate around (0, 1, 0)
    float thetaX = m_angleX;
    glm::mat4 rot_x = glm::mat4(glm::vec4(glm::cos(thetaX), 0.f, -glm::sin(thetaX), 0.f),
                                glm::vec4(0.f, 1.f, 0.f, 0.f),
                                glm::vec4(glm::sin(thetaX), 0.f, glm::cos(thetaX), 0.f),
                                glm::vec4(0.f, 0.f, 0.f, 1.f));

    // y: rotate around look cross up
    float thetaY = m_angleY;
    glm::vec3 look = m_camera.getCameraLook();
    glm::vec3 up = m_camera.getCameraUp();
    glm::vec3 u = glm::normalize(glm::cross(look, up));
    glm::mat4 rot_y = glm::mat4(glm::vec4(glm::cos(thetaY) + u.x * u.x * (1 - glm::cos(thetaY)),
                                          u.x * u.y * (1 - glm::cos(thetaY)) + u.z * glm::sin(thetaY),
                                          u.x * u.z * (1 - glm::cos(thetaY)) - u.y * glm::sin(thetaY),
                                          0.f),
                                glm::vec4(u.x * u.y * (1 - glm::cos(thetaY)) - u.z * glm::sin(thetaY),
                                          glm::cos(thetaY) + u.y * u.y * (1 - glm::cos(thetaY)),
                                          u.y * u.z * (1 - glm::cos(thetaY)) + u.x * glm::sin(thetaY),
                                          0.f),
                                glm::vec4(u.x * u.z * (1 - glm::cos(thetaY)) + u.y * glm::sin(thetaY),
                                          u.y * u.z * (1 - glm::cos(thetaY)) - u.x * glm::sin(thetaY),
                                          glm::cos(thetaY) + u.z * u.z * (1 - glm::cos(thetaY)),
                                          0.f),
                                glm::vec4(0.f, 0.f, 0.f, 1.f));

    look = rot_x * rot_y * glm::vec4(look, 0.f);
    up = rot_x * rot_y * glm::vec4(up, 0.f);

    // update view matrix
    m_camera.setViewMatrix(glm::normalize(look), glm::normalize(up), m_camera.getCameraPos());
    m_view = m_camera.getViewMatrix();
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around
    glm::vec3 dir = glm::vec3(0.f);
    float t = 5 * deltaTime;

    glm::vec3 look = m_camera.getCameraLook();
    glm::vec3 up = m_camera.getCameraUp();

    if (m_keyMap[Qt::Key_W]){
        // Translates the camera in the direction of the look vector
        dir += look;
    }
    if (m_keyMap[Qt::Key_S]){
        // Translates the camera in the opposite direction of the look vector
        dir += -look;
    }
    if (m_keyMap[Qt::Key_A]){
        // Translates the camera in the left direction, perpendicular to the look and up vectors
        dir += glm::cross(up, look);
    }
    if (m_keyMap[Qt::Key_D]){
        // Translates the camera in the right direction, also perpendicular to the look and up vectors.
        dir += glm::cross(look, up);
    }
    if (m_keyMap[Qt::Key_Space]){
        // Translates the camera along the world space vector (0, 1, 0)
        dir += glm::vec3(0.f, 1.f, 0.f);
    }
    if (m_keyMap[Qt::Key_Control]){
        // Translates the camera along the world space vector (0, -1, 0)
        dir += glm::vec3(0.f, -1.f, 0.f);
    }

    if (dir == glm::vec3(0.f)){
        return;
    }

    dir = glm::normalize(dir);
    glm::vec3 pos = glm::vec3(m_camera.getCameraPos()) + t * dir;

    // update view matrix
    m_camera.setViewMatrix(glm::normalize(look), glm::normalize(up), pos);
    m_view = m_camera.getViewMatrix();

    update(); // asks for a PaintGL() call to occur
}

// ================== Final Project: shadow mapping

//    // 1. first render to depth map
//    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//        glClear(GL_DEPTH_BUFFER_BIT);
//        ConfigureShaderAndMatrices();
//        RenderScene();
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    // 2. then render scene as normal with shadow mapping (using depth map)
//    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    ConfigureShaderAndMatrices();
//    glBindTexture(GL_TEXTURE_2D, depthMap);
//    RenderScene();

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
    glGenFramebuffers(1, &m_depth_fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, m_depth_fbo);

    // add texture as a depth attachment to depth FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

void Realtime::lightSpaceTransform(SceneLightData light){
    // TODO: how to do choose near and far
    float near_plane = 0.1f, far_plane = 100.f;

    // pos for dir light: 3 from the center of scene
    glm::vec3 lightPos = -glm::normalize(glm::vec3(light.dir)) * 6.f;

    // orthographic projection for directional light
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

    // light source position looking at scene's center
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (light.dir == glm::vec4(0.f, -1.f, 0.f, 0.f) || light.dir == glm::vec4(0.f, 1.f, 0.f, 0.f)){
        up = glm::vec3(1.0f, 1.0f, 0.0f);
    }
    glm::mat4 lightView = glm::lookAt(lightPos,
                                      glm::vec3( 0.0f, 0.0f, 0.0f),
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

    // set uniforms
    // light space transformation matrix
    GLint lightSpaceMatrixLocation = glGetUniformLocation(m_depth_shader, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &m_lightSpaceMatrix[0][0]);

    for (RenderShapeData const &shapeData : m_metaData.shapes){
        m_model = shapeData.ctm;

        // set uniform
        // matrices
        GLint modelMatrixLocation = glGetUniformLocation(m_depth_shader, "mat_model");
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &m_model[0][0]);

        // draw
        if (m_eagle_shadow_on){
            m_eagle_vao.draw(m_eagleData.size(), -1, -1);
        } else{
            switch(shapeData.primitive.type){
            case PrimitiveType::PRIMITIVE_CUBE:
                m_cube_vao.draw(m_cubeData.size(), -1, -1);
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_sphere_vao.draw(m_sphereData.size(), -1, -1);
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_cylinder_vao.draw(m_cylinderData.size(), -1, -1);
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                m_cone_vao.draw(m_coneData.size(), -1, -1);
                break;
            default:
                break;
            }
        }
    }

    glUseProgram(0);
}

// Inserts a glm::vec3 into a vector of floats.
void Realtime::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

// Inserts a glm::vec2 into a vector of floats.
void Realtime::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}

void Realtime::load(const std::string &path) {

    m_eagleData.clear();

    std::string warn, err;
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;

    bool success = reader.ParseFromFile(path);
    if (!success) {
        std::cerr << "load obj fail: " << reader.Error() << std::endl;
        return;
    }

    if (!reader.Warning().empty()) {
        std::cout << "load obj warning: " << reader.Warning() << std::endl;
    }

    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();
    const auto &materials = reader.GetMaterials();

    // loop over shapes
    for (const auto &shape : shapes) {
        size_t indexOffset = 0;

        std::cout << "number of faces: " << shape.mesh.num_face_vertices.size() << std::endl;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            size_t fv = shape.mesh.num_face_vertices[f];

            std::cout << "number of vertices for curr face: " << fv << std::endl;

            if (fv != 3) {
                std::cout << "not triangle" << std::endl;
            }

            // loop over vertices in the face
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                insertVec3(m_eagleData, glm::vec3(vx, vy, vz));

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    insertVec3(m_eagleData, glm::vec3(nx, ny, nz));
                } else {
                    // use normal (0, 1, 0) if obj does not have normal specified.
                    insertVec3(m_eagleData, glm::vec3(0, 1, 0));
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    insertVec2(m_eagleData, glm::vec2(tx, ty));
                } else {
                    // use uv coordinate (0, 0) if obj does not have uv specified.
                    insertVec2(m_eagleData, glm::vec2(0, 0));
                }
            }
            indexOffset += fv;

            // access to material of the face
            if (shape.mesh.material_ids[f] >= 0) {

            }
        }
    }
    return;
}
