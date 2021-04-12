#version 330

out vec4 frag_color;

in VertexData {
    vec3 normal;
} VertexIn;

void main() {

    frag_color = vec4(0.0,0.3,0.0,1.0);
    
}