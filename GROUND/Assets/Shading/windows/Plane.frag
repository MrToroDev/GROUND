#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec2 Texcoord, UV; // for basic 2D / 3D objects
    vec3 Color, Normal, FragPos, Pos;
} vs_out;

uniform sampler2D uTexture;

void main()
{
	FragColor = texture(uTexture, vs_out.Texcoord);
}