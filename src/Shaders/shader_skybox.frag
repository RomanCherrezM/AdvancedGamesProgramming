#version 330 core
out vec4 FragColor;

in vec3 in_uv;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, in_uv);
}