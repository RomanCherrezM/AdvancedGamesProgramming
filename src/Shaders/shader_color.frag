#version 330

out vec4 frag_color;

uniform vec4 color;
in vec3 in_normal;

void main() {

    // Light Diffuse
    vec3 lightDir = normalize(vec3(-0.35,-0.4,-0.6));
    float diff = max(dot(abs(in_normal), -lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0,1.0,1.0);

    // Light Ambient
    float ambient = 0.3;
    vec3 total_light = ambient + diffuse;

    frag_color = color * vec4(total_light,1.0);
    
}