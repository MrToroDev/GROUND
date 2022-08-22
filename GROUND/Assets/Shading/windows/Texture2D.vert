#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexcoord;

out VS_OUT {
	vec3 Pos, Color;
	vec2 Texcoord;
} vs_out;

uniform mat4 model;

void main()
{
	vs_out.Texcoord = aTexcoord;
	vs_out.Pos = aPos; 
	vs_out.Color = aColor;
	gl_Position = model * vec4(aPos, 1.0);
}