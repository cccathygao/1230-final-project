#version 330 core

layout(location=0) in vec3 objectPosition;
layout(location=1) in vec3 objectNormal;
out vec3 worldPosition;
out vec3 worldNormal;

uniform mat4 modelMat;
uniform mat4 worldNormalMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
    //position output
    worldPosition = vec3(modelMat*vec4(objectPosition,1.f));
    worldNormal = mat3(worldNormalMat)*objectNormal;
    gl_Position = projMat*viewMat*vec4(worldPosition,1.0);
}
