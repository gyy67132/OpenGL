#version 430

out vec4 fragColor;

in vec2 outTexcoord;

uniform sampler2D image;
uniform vec3 color;

void main()
{
	fragColor = vec4(color, 1.0) * texture(image, outTexcoord);
}