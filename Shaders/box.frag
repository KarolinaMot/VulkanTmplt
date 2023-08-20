#version 450

layout(location = 1) in vec3 textureDir;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 2) uniform samplerCube cubemap;

void main() {
    // Sample the diffuse texture color
    outColor = texture(cubemap, textureDir);
}

//#version 450
//layout(location = 0) in vec3 fragColor;
//layout(location = 1) in vec2 fragTexCoord;
//
//layout(location = 0) out vec4 outColor;
//
//layout(set = 1, binding = 1) uniform sampler2D texSampler;
//
//void main() {
//    outColor = texture(texSampler, fragTexCoord);
//}
