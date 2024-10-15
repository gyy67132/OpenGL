#version 330

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texcoords;

uniform mat4 lightSpaceMatrix;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(vertex.xy, 0.0, 1.0);
	TexCoord = texcoords;
}