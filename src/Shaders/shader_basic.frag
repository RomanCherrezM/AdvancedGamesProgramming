#version 330

out vec4 frag_color;

in vec3 in_normal;
in vec2 in_uv;

uniform sampler2D base_texture;

void main() {
    frag_color = texture(base_texture,in_uv);
    
}