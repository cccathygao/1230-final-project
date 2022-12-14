#version 330 core
layout (location = 0) in vec3 position_obj;

uniform mat4 lightSpaceMatrix;
uniform mat4 mat_model;

void main()
{
    // convert position from obj space to light space
    gl_Position = lightSpaceMatrix * mat_model * vec4(position_obj, 1.0);
}
