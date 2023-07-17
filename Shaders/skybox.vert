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

const vec2 screen_corners[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0),
    vec2( 1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0, -1.0)
);

void main() {
    vec2 pos = screen_corners[gl_VertexIndex];
    gl_Position = vec4(pos, 0.0, 1.0);
}