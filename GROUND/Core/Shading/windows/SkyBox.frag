#version 460 core

out vec4 FragColor;

in vec3 textureCoord;

uniform samplerCube SkyBox0;

void main()
{
	FragColor = texture(SkyBox0, textureCoord);
}