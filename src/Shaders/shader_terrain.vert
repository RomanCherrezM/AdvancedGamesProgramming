#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 in_position;
out vec3 in_normal;
out vec2 in_uv;

void main() {
    gl_Position = projection * view * model * (vec4(position ,1.0));
    in_normal = normalize(normal);
    in_position = position;
    in_uv = uv * 20;
}