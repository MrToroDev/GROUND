#version 460 core

uniform sampler2D uTexture;
uniform float _clock;
in vec2 vTexcoords;
out vec4 FragColor;

vec2 PixelateFrame(vec2 UV, float noise);
vec4 TVScreenEffect(vec2 texcoords, sampler2D texture);
void GrayScale(out vec4 FragColor);
vec3 StaticTVNoise(vec2 texcoords, float _clock);
vec4 KernelEffect(sampler2D uTexture, vec2 vTexcoords, float noise);

void main()
{
    FragColor = TVScreenEffect(vTexcoords, uTexture);

//    FragColor = texture(uTexture, vTexcoords);
}