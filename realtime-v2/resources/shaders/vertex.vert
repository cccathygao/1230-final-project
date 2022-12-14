#version 330 core

layout(location=0) in vec3 objectPosition;
layout(location=1) in vec3 objectNormal;
layout(location=2) in vec3 objectColor;

out vec3 worldPosition;
out vec3 worldNormal;
out vec3 outColor;

uniform mat4 modelMat;
uniform mat4 worldNormalMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
    outColor = objectColor;
    vec3 realpos = vec3(objectPosition[0],objectPosition[2],objectPosition[1]);
    vec3 realNorm = vec3(objectNormal[0],objectNormal[2],objectNormal[1]);
    //position output
    worldPosition = vec3(modelMat*vec4(realpos,1.f));
    worldNormal = mat3(worldNormalMat)*normalize(realNorm);
    gl_Position = projMat*viewMat*vec4(worldPosition,1.0);

}

