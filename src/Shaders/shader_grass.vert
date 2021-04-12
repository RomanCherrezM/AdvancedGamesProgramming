#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out VertexData {
    vec3 normal;
    mat4 proj;
} VertexOut;

out vec3 in_normal;

void main() {
    VertexOut.proj = projection * view;
    gl_Position = model* (vec4(position ,1.0));
    VertexOut.normal = normal;
}