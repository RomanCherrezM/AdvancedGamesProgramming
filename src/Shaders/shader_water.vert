#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform float time;
uniform sampler2D height_map1;
uniform sampler2D normal_map1;
uniform sampler2D height_map2;
uniform sampler2D normal_map2;

out vec3 in_position;
out vec3 in_normal;
out vec2 in_uv;

void main() {

    float speed = 0.00005;
    float amount = 0.02;
    float extension = 1.0;

    vec2 v_uv = uv * time/10000.0;

    float y1 = texture2D(height_map1,v_uv + time * speed + extension).r * amount;
    vec3 text_normal1 = texture2D(normal_map1,v_uv + time * speed + extension).rgb;
    float y2 = texture2D(height_map2,v_uv - time * speed + extension).r * amount;
    vec3 text_normal2 = texture2D(normal_map2,v_uv - time * speed + extension).rgb;

    float height = (y1*0.5 + y2*0.5);
    vec3 final_normal = normalize(text_normal1 + text_normal2);

    in_normal = mat3(transpose(inverse(model))) * final_normal;
    in_position = vec3(model * vec4(position.x,height,position.z, 1.0));
    in_uv = v_uv;

	gl_Position = projection * view * vec4(in_position,1.0);

}