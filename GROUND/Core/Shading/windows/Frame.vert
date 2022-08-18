#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexcoord;

out vec2 vTexcoords;

void main()
{
	mat4 Result = mat4(1), m = Result;
	vec3 v = vec3(2);

    Result[0] = m[0] * v[0];
	Result[1] = m[1] * v[1];
	Result[2] = m[2] * v[2];
	Result[3] = m[3];
    gl_Position =   Result * vec4(aPos, 1.0);
    vTexcoords = aTexcoord;
}