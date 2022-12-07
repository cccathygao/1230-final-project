#version 330 core

layout(location = 0) in vec3 position_obj;
layout(location = 1) in vec3 normal_obj;
layout(location = 2) in vec2 uv_obj;

out vec3 position_world;
out vec3 normal_world;
out vec2 uv;

uniform mat4 mat_model;
uniform mat3 mat_model_inv_trans;

uniform mat4 mat_view;
uniform mat4 mat_proj;

void main() {
    position_world = vec3(mat_model * vec4(position_obj, 1.f));
    normal_world = mat_model_inv_trans * normalize(normal_obj);
    uv = uv_obj;
    gl_Position = mat_proj * mat_view * mat_model * vec4(position_obj, 1.f);
}
