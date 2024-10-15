#version 330

uniform sampler2D depthMap ;

out vec4 color;
in vec2 TexCoord;

void main()
{
	float depthValue = texture(depthMap, TexCoord).r;
	color = vec4(vec3(depthValue), 1.0);
}