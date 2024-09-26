#version 430

out vec4 fragColor;

in vec4 varyingColor;
in vec2 outTexcoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	fragColor = texture(texture_diffuse1, outTexcoord);
}