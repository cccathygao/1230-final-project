#version 330 core

// 0: pos, 1: uv
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_in;

out vec2 uv;

void main()
{
    uv = uv_in;
    gl_Position = vec4(position, 1.0);
}
