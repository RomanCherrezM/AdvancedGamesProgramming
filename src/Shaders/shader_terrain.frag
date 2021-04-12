#version 330

out vec4 frag_color;

in vec3 in_position;
in vec3 in_normal;
in vec2 in_uv;

uniform sampler2D base_texture; 
uniform sampler2D grass_texture;
uniform sampler2D sand_texture;

void main() {

    vec3 base_color = texture(base_texture,in_uv).rgb;
    vec3 grass_color = texture(grass_texture,in_uv).rgb;
    vec3 sand_color = texture(sand_texture,in_uv).rgb;

    float s = clamp(smoothstep(0.0,1.0,in_position.y * 2.0),0.0,1.0);
    float s2 = clamp(smoothstep(0.0,1.0,in_position.y),0.0,1.0);

    vec3 final_color = mix(sand_color,base_color,s);
    final_color = mix(final_color,grass_color,s2);

    // Light Diffuse
    vec3 lightDir = normalize(vec3(-0.35,-0.4,-0.6));
    float diff = max(dot(in_normal, -lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0,1.0,1.0);

    // Light Ambient
    float ambient = 0.3;
    vec3 total_light = ambient + diffuse;

    frag_color = vec4(final_color * total_light,1.0);
    
}