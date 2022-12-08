#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvin;
out vec2 uvpass;

void main() {
    uvpass = uvin;
    gl_Position = vec4(position, 1.0);

}
