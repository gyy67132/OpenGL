#version 430

layout(location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec4 Color;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 ParticleColor;

void main()
{
	float scale = 10.0f;
	TexCoords = vertex.zw;
	gl_Position = projection * vec4(vertex.xy * scale + offset, 0.0, 1.0);
	Color = ParticleColor;
}