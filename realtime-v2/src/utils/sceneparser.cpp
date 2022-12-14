
#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    // TODO: Use your code from Lab 5 here.
    renderData.globalData = fileReader.getGlobalData();
    renderData.lights.clear();
    renderData.lights = fileReader.getLights();
    renderData.cameraData = fileReader.getCameraData();
    SceneNode *root = fileReader.getRootNode();
    renderData.shapes.clear();

    // Task 5: populate renderData's list of primitives and their transforms.
    //         This will involve traversing the scene graph, and we recommend you
    //         create a helper function to do so!

    dfs(root,renderData.shapes,glm::mat4(1.0f));
    return true;
}

void SceneParser::dfs(SceneNode *root, std::vector<RenderShapeData> &shapes, glm::mat4 cmt){

    glm::mat4 tr = glm::mat4(1.0f);
    for(int i = 0; i<root->transformations.size();i++){
        TransformationType type = root->transformations[i]->type;
        glm::mat4 m;
        if(type == TransformationType::TRANSFORMATION_ROTATE){
            m = glm::rotate(root->transformations[i]->angle,root->transformations[i]->rotate);
        }
        else if(type == TransformationType::TRANSFORMATION_SCALE){
            m = glm::scale(root->transformations[i]->scale);
        }
        else if(type == TransformationType::TRANSFORMATION_TRANSLATE){
            m = glm::translate(root->transformations[i]->translate);
        }
        else{
            m = root->transformations[i]->matrix;
        }
        if(i==0){
            tr = m;
        }
        else{
            tr = tr * m;
        }
    }
    glm::mat4 newcmt = cmt*tr;
    if(root->children.size() == 0){
        for(int i = 0; i<root->primitives.size();i++){
            shapes.insert(shapes.end(), RenderShapeData{*root->primitives[i],newcmt,glm::inverse(glm::transpose(newcmt))});
        }
        return;
    }
    for(int i=0; i<root->children.size(); i++){
        dfs(root->children[i],shapes,newcmt);
    }

}
