#version 460 core

uniform sampler2D uTexture;
uniform float _clock;
in vec2 vTexcoords;
out vec4 FragColor;

vec2 PixelateFrame(vec2 UV, float noise);

void main()
{
    FragColor = texture(uTexture, PixelateFrame(vTexcoords, 255));
//    FragColor = texture(uTexture, vTexcoords);
}