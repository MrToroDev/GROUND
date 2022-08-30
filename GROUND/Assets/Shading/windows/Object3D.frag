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

vec3 DrawPointLight_raw(vec3 vNormal, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue);

void main() {
//    FragColor = mix(vec4(0.3, 0.3, 0.3, 1.0), FragColor, FogVisibility);

    FragColor = vec4(DrawPointLight_raw(fs_in.Normal, vs_light_out.ViewPos, vec3(1), vs_light_out.ViewPos, fs_in.FragPos, 0.5, 2, 32), 1.0);
}