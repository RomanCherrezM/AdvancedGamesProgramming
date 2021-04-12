#version 330 
layout (points) in;
layout (triangle_strip, max_vertices = 42) out;

uniform float time;

vec4 offsets[7];

in VertexData {
    vec3 normal;
    mat4 proj;
} VertexIn[];
 
out VertexData {
    vec3 normal;
} VertexOut;

void main() {    

    vec4 position = gl_in[0].gl_Position;
    offsets[0] = position;
    offsets[1] = position + vec4(0.4,0.0,0.0,0.0);
    offsets[2] = position + vec4(-0.4,0.0,0.0,0.0);
    offsets[3] = position + vec4(0.0,0.0,0.4,0.0);
    offsets[4] = position + vec4(0.0,0.0,-0.4,0.0);
    offsets[5] = position + vec4(0.2,0.0,0.2,0.0);
    offsets[6] = position + vec4(-0.2,0.0,-0.2,0.0);

    for(int i=0;i<7;++i){
        float t = sin(time * 0.001 + position.x * 0.1) * 0.5;
        float t2 = cos(time * 0.001 + position.x * 0.1) * 0.5;

        gl_Position = VertexIn[0].proj * ( position + offsets[i] + vec4(-0.02, 0.0, 0.0, 0.0)); 
        VertexOut.normal = VertexIn[0].normal;
        EmitVertex();   
        gl_Position = VertexIn[0].proj *( position + offsets[i] + vec4( 0.1, 0.0, 0.0, 0.0));
        VertexOut.normal = VertexIn[0].normal;
        EmitVertex();
        gl_Position = VertexIn[0].proj * ( position + offsets[i] + vec4(t, 2.0,-t2, 0.0)); 
        VertexOut.normal = VertexIn[0].normal;
        EmitVertex();      
        EndPrimitive();
    }  

    for(int i=0;i<7;++i){
        float t = sin(time * 0.001 + position.x * 0.1) * 0.5;
        float t2 = cos(time * 0.001 + position.x * 0.1) * 0.5;

        gl_Position = VertexIn[0].proj * ( position + offsets[i] + vec4(-0.02, 0.0, 0.0, 0.0)); 
        VertexOut.normal = VertexIn[0].normal;
        EmitVertex();
        gl_Position = VertexIn[0].proj * ( position + offsets[i] + vec4(t, 2.0,-t2, 0.0)); 
        VertexOut.normal = VertexIn[0].normal;
        EmitVertex();      
        gl_Position = VertexIn[0].proj *( position + offsets[i] + vec4( 0.1, 0.0, 0.0, 0.0));
        VertexOut.normal = VertexIn[0].normal;
        EmitVertex();
        EndPrimitive();   
    } 
}  