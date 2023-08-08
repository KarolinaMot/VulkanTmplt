#version 450

layout(set = 0, binding = 0) uniform VPMatrix {
    mat4 view;
    mat4 proj;
} vp;

layout(set = 1, binding = 0) uniform ModelMatrix {
    mat4 model;
} m;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normal;

void main() {
    gl_Position = vp.proj * vp.view * m.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    mat3 normalMatrix = transpose(inverse(mat3(m.model)));
    normal = normalMatrix * inNormal;
}