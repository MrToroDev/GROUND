#version 330 core

out vec4 FragColor;
in vec3 vLightColor, vViewPos, vLightPos, vViewFront, vColor, vNormal, FragPos, vPos;
uniform mat4 model;
vec3 DrawPointLight(vec3 vNormal, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue);

void main() {
	FragColor = vec4(0.5 * vLightColor, 1.0);
}