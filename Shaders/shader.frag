#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 1) uniform sampler2D texSampler;

void main() {

    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));

    // Calculate the diffuse factor
    float diffuseFactor = max(dot(normal, lightDir), 0.1);

    // Sample the diffuse texture color
    vec4 diffuseTextureColor = texture(texSampler, fragTexCoord);

    // Calculate the final color
    vec3 diffuseColor = vec3(1.0, 0.5, 0.5) * diffuseTextureColor.rgb * diffuseFactor;


    outColor = vec4(diffuseColor, diffuseTextureColor.a);

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
