#version 330 core

out vec4 FragColor;
uniform mat4 model;

in VS_OUT {
    vec2 Texcoord, UV; // for basic 2D / 3D objects
    vec3 Color, Normal, FragPos, Pos;
} vs_out;

in VS_LIGHT_OUT {
    vec3 LightColor, ViewPos, LightPos, ViewFront;
} vs_light_out;

vec3 DrawPointLight(vec3 vNormal, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue);
vec3 DrawDirectionalLight(vec3 vNormal, vec3 lightColor, vec3 viewPos, vec3 FragPos, vec3 direction, float ambientStrength, float specularStrength, float impactValue);

void main() {
	float gamma = 2.2;
	FragColor = vec4(DrawPointLight(vs_out.Normal, vs_light_out.ViewPos, vec3(1, 1, 1), vs_light_out.ViewPos, vs_out.FragPos, 0.5, 2, 32), 1);

	FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
	
}