#version 430

out vec4 fragColor;

in vec4 varyingColor;

void main()
{
	fragColor = varyingColor;
}