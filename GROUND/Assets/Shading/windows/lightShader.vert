#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexcoord;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec2 aUV;

out VS_OUT {
    vec2 Texcoord, UV; // for basic 2D / 3D objects
    vec3 Color, Normal, FragPos, Pos;
} vs_out;

out VS_LIGHT_OUT {
    vec3 LightColor, ViewPos, LightPos, ViewFront;
} vs_light_out;

uniform mat4 projection, model, view;
uniform vec3 lightColor, viewPos, lightPos, viewFront;

out float FogVisibility;

const float FogDensity = 0.5;
const float FogGradient = 1.5;

void main()
{
    vs_out.Pos = aPos;
    vs_out.Texcoord = aTexcoord;
    vs_out.Color = aColor;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.UV = aUV;

    vs_light_out.LightColor = lightColor;
    vs_light_out.ViewPos = viewPos;
    vs_light_out.LightPos = lightPos;
    vs_light_out.ViewFront = viewFront;

    vec4 posRelativeToCam = view * (model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    float distance = length(posRelativeToCam.xyz);
    FogVisibility = exp(-pow((distance * FogDensity), FogGradient));
}