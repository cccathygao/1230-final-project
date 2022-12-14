#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inColor;
out vec4 vert;
out vec4 norm;
out vec3 color;
out vec3 lightDir;

uniform mat4 projMatrix;
uniform mat4 mvMatrix;

void main()
{
    vec3 vertex1 = vec3(vertex[0],vertex[2],vertex[1]);
    vec3 normal1 = vec3(normal[0],normal[2],normal[1]);
    vert  = mvMatrix * vec4(vertex1, 1.0);
    norm  = transpose(inverse(mvMatrix)) *  vec4(normal1, 0.0);
    color = inColor;
    lightDir = normalize(vec3(1, -1, 1));
    gl_Position = projMatrix * mvMatrix * vec4(vertex1, 1.0);
}
