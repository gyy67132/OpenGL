#version 430

out vec4 fragColor;

in vec2 TexCoords;
in vec4 Color;

uniform sampler2D sprite;


void main()
{
	fragColor = Color * texture(sprite, TexCoords);
}