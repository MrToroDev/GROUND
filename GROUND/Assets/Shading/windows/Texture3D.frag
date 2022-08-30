#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec2 Texcoord, UV; // for basic 2D / 3D objects
    vec3 Color, Normal, FragPos, Pos;
} fs_in;

in VS_LIGHT_OUT {
    vec3 LightColor, ViewPos, LightPos, ViewFront;
} vs_light_out;

in float FogVisibility;

uniform sampler2D texture_normal1;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec3 DrawPointLight(sampler2D normalMap, sampler2D diffuseMap, sampler2D specularMap, vec2 texcoord, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue);
vec3 DrawPointLight_raw(vec3 vNormal, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue);

void main() {
    FragColor = vec4(DrawPointLight(texture_normal1, texture_diffuse1, texture_specular1, fs_in.UV, vs_light_out.ViewPos, vec3(1), vs_light_out.ViewPos, fs_in.FragPos, 0.5, 1, 32), 1.0);
//    FragColor = mix(vec4(0.3, 0.3, 0.3, 1.0), FragColor, FogVisibility);

//    FragColor = vec4(DrawPointLight_raw(fs_in.Normal, vs_light_out.ViewPos, vec3(1), vs_light_out.ViewPos, fs_in.FragPos, 0.5, 2, 32), 1.0);
}