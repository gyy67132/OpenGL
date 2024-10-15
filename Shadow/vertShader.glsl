#version 330

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

out VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normals;
	vec4 FragPosLightSpace;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(vertex, 1.0);
	vs_out.TexCoords = texcoord;
	vs_out.Normals = transpose(inverse(mat3(model))) * normal;
	vs_out.FragPos = vec3(model * vec4(vertex, 1.0));
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}