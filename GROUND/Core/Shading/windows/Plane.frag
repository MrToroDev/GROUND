#version 330 core

out vec4 FragColor;
in vec2 vTexcoord;
uniform sampler2D uTexture0;
uniform mat4 model;
in vec3 vLightColor, vViewPos, vLightPos, vViewFront, vColor, vNormal, FragPos, vPos;

vec3 DrawPointLight(vec3 vNormal, vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 FragPos, float ambientStrength, float specularStrength, float impactValue);

void main()
{
    vec4 pixelLight = vec4(DrawPointLight(vNormal, vViewPos, vec3(1.0, 1.0, 1.0), vViewPos, FragPos, 0.5, 2, 16), 1.0) * texture(uTexture0, vTexcoord);
	FragColor = pixelLight;
}