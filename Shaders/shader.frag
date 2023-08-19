#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 viewPos;
layout(location = 4) in vec3 pos;


layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 1) uniform sampler2D diffuseTex;
layout(set = 1, binding = 2) uniform sampler2D specularTex;
float specularStrength = 0.2;


void main() {

    vec3 normalizedNormal = normalize(normal);
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, normal);  

    // Calculate the diffuse factor
    float diff = max(dot(normalizedNormal, lightDir), 0.1);

    // Calculate the specular factor using the Phong lighting model
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 64) * specularStrength;

    // Sample the diffuse texture color
    vec4 diffuseTextureColor = texture(diffuseTex, fragTexCoord);
    vec4 specularTextureColor = texture(specularTex, fragTexCoord);

    // Calculate the final color
    vec3 diffuseCol = vec3(1.0, 0.5f, 0.5f) * diffuseTextureColor.rgb * diff;
    vec3 specularCol = vec3(1.0, 0.5f, 0.5f) * spec * specularTextureColor.rgb; // Use a white specular color


    vec3 finalColor = diffuseCol + specularCol;
    outColor = vec4(finalColor, diffuseTextureColor.a);  

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
