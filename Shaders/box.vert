#version 450

layout(set = 0, binding = 0) uniform VPMatrix {
    mat4 view;
    mat4 proj;
} vp;

layout(set = 1, binding = 0) uniform ModelMatrix {
    mat4 model;
} m;

layout(location = 0) in vec3 inPosition;

layout(location = 1) out vec3 fragTexCoord;

void main() {
    fragTexCoord = inPosition;
    vec4 pos = vp.proj * vp.view * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;
}