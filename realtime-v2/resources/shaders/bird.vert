#version 330 core

layout(location=0) in vec3 objectPosition;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;
//layout(location=1) in vec3 objectNormal;
out vec2 uvpass;

uniform mat4 modelMat;
//uniform mat4 worldNormalMat;
uniform mat4 vp;


void main() {
    //position output
    uvpass = uv;
    vec3 worldPosition = vec3(modelMat*vec4(objectPosition,1.f));
    gl_Position = vp*vec4(worldPosition,1.0);
}
