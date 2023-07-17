#version 450
layout(location = 0) in vec3 inPosition;

 layout(set = 0, binding = 0) uniform CameraVectors {
    vec3 forwards;
    vec3 right;
    vec3 up;
} vectors;

layout( location = 0 ) out vec3 forwards;

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

    forwards = normalize(vectors.forwards + pos.x * vectors.right - pos.y * vectors.up).xyz;
}