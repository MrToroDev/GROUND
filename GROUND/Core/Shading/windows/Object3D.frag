#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec2 Texcoord, UV; // for basic 2D / 3D objects
    vec3 Color, Normal, FragPos, Pos;
} vs_out;

in VS_LIGHT_OUT {
    vec3 LightColor, ViewPos, LightPos, ViewFront;
} vs_light_out;

vec3 DrawPointLight(vec3 vNormal, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue);

void main() {
	FragColor = vec4(DrawPointLight(vs_out.Normal, vs_light_out.ViewPos, vec3(1), vs_light_out.ViewPos, vs_out.FragPos, 0.5, 1, 32), 1) * vec4(vs_out.Color, 1.0);
}