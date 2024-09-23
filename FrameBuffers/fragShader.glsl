#version 430

out vec4 fragColor;

in vec4 varyingColor;
in vec2 outTexcoord;

uniform sampler2D texture1;

void main()
{
	fragColor = texture(texture1, outTexcoord);
}