#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

void dfs(SceneNode *root, glm::mat4 ctm, std::vector<RenderShapeData> &shapes){

    if (root == nullptr){
        return;
    }

    for(SceneTransformation* t : root->transformations){
        switch(t->type){
        case TransformationType::TRANSFORMATION_TRANSLATE:
            ctm = ctm * glm::translate(t->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            ctm = ctm * glm::scale(t->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            ctm = ctm * glm::rotate(t->angle, t->rotate);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            ctm = ctm * t->matrix;
            break;
        default:
            break;
        }
    }

    for(ScenePrimitive* p : root->primitives){
        shapes.push_back(RenderShapeData{.primitive = *p, .ctm = ctm});
    }

    for(SceneNode* node : root->children){
        dfs(node, ctm, shapes);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    // TODO: Use your code from Lab 5 here.

    // Task 4: populate renderData with global data, lights, and camera data
    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();
    renderData.lights = fileReader.getLights();

    // Task 5: populate renderData's list of primitives and their transforms.
    //         This will involve traversing the scene graph, and we recommend you
    //         create a helper function to do so!
    SceneNode *root = fileReader.getRootNode();
    renderData.shapes.clear();
    glm::mat4 ctm(1.0f);
    dfs(root, ctm, renderData.shapes);

    return true;
}
