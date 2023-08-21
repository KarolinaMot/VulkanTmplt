#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 viewPos;
layout(location = 4) in vec3 pos;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform LightInfo{
	vec3 pos;
	vec3 direction;
	vec4 color;
    float radius;
	int type;
} light;

layout(set = 2, binding = 1) uniform sampler2D diffuseTex;
layout(set = 2, binding = 2) uniform sampler2D specularTex;
float specularStrength = 0.2;
float ambient = 0.8f;

vec3 CalculateLight(vec4 color, vec3 direction, vec3 norm, vec3 viewDir, vec4 diffuseTex, vec4 metallicTex){
    vec3 ambient = ambient * color.rgb * vec3(diffuseTex);
 
    float diff = max(dot(norm, direction), 0.0);
    vec3 diffuse = diff  * vec3(diffuseTex);

    vec3 reflectDir = reflect(-direction, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.f);
    vec3 specular = spec * color.rgb * vec3(metallicTex);  

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(vec3 position, float radius, vec4 color, vec3 norm, vec3 viewDir, vec4 diffuseTex, vec4 metallicTex){
    float  dist = length(position - pos);
    float att = clamp(1.0 - dist/light.radius, 0.0, 1.0);
    att *= att;

    vec3 direction = normalize(position - pos);
    return att * CalculateLight(color, direction, norm, viewDir, diffuseTex, metallicTex);
}


void main() {

    vec4 diffuseTextureColor = texture(diffuseTex, fragTexCoord);
    vec4 specularTextureColor = texture(specularTex, fragTexCoord);
    vec3 viewDir = normalize(viewPos - pos);
    vec3 norm = normalize(normal);

    vec3 normalizedNormal = normalize(normal);
    vec3 reflectDir = reflect(-light.direction, normal);  


    vec3 result = CalculatePointLight(light.pos, light.radius, light.color, norm, viewDir, diffuseTextureColor, specularTextureColor);
    outColor = vec4(result, 1.0);
}