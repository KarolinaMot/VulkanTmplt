#version 450
layout(location = 0) in vec3 inPosition;

 layout(set = 0, binding = 0) uniform VPMatrix {
    mat4 view;
    mat4 proj;
} vp;

layout(set = 1, binding = 0) uniform ModelMatrix {
    mat4 model;
} m;

layout( location = 0 ) out vec3 vert_texcoord;

 void main() {
     vec3 position = mat3(vp.view * m.model) * inPosition.xyz;
     gl_Position = (vp.proj * vec4( position, 0.0 )).xyzz;
     vert_texcoord = inPosition.xyz;
 }