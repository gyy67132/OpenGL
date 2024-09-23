#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 varyingColor;
out vec2 outTexcoord;

void main()
{
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
	outTexcoord = texcoord;
}