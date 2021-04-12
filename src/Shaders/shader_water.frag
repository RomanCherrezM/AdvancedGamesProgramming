#version 330

out vec4 frag_color;

in vec3 in_position;
in vec3 in_normal;
in vec2 in_uv;

uniform vec3 camera_pos;
uniform samplerCube cubemap;

void main() {

    // Light Specular
    float specularStrength = 1.0;
    vec3 lightDir = normalize(vec3(-0.35,-0.4,-0.6));
    vec3 viewDir = normalize(camera_pos - in_position);
    vec3 reflectDir = reflect(-lightDir, in_normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0,1.0,1.0);  

    // Light Diffuse
    float diff = max(dot(in_normal, -lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0,1.0,1.0);

    // Light Ambient
    float ambient = 0.3;
    vec3 total_light = specular + ambient + diffuse;

    vec3 color = vec3(0.3,0.6,0.8);

    //Cubemap 
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(in_position - camera_pos);
    vec3 R = refract(I, normalize(in_normal), ratio);

    vec3 ref_color = texture(cubemap, R).rgb; 
    vec3 final_color = mix(color,ref_color,0.8);

    frag_color = vec4(final_color * total_light, 0.9);

}