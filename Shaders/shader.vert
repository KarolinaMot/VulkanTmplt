#version 450

layout(set = 0, binding = 0) uniform VPMatrix {
    mat4 view;
    mat4 proj;
    vec4 viewPos;
} vp;

layout(set = 2, binding = 0) uniform ModelMatrix {
    mat4 model;
} m;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 tangent;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 viewPos;
layout(location = 4) out vec3 pos;
layout(location = 5) out mat3 TBN;

void main() {

    vec3 T = normalize(vec3(m.model * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(m.model * vec4(inNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    gl_Position = vp.proj * vp.view * m.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    mat3 normalMatrix = transpose(inverse(mat3(m.model)));
    normal = normalMatrix * inNormal;
    viewPos = vp.viewPos.xyz;
    pos = (m.model * vec4(inPosition, 1.0)).xyz;
}