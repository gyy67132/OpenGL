#version 430

layout(location = 0) in vec4 positionAndTexcoord;

uniform mat4 projection;
uniform mat4 model;

out vec2 outTexcoord;

void main()
{
	gl_Position = projection * model * vec4(positionAndTexcoord.xy, 0.0, 1.0);
	outTexcoord = positionAndTexcoord.zw;
}