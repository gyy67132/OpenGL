#version 430

out vec4 color;

in vec2 TexCoords;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;


void main()
{
	color = vec4(0.0);
	vec3 ssample[9];
	if (shake || chaos)
	{
		for (int i = 0; i < 9; i++)
			ssample[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
	}

	if (chaos)
	{
		for (int i = 0; i < 9; i++)
			color += vec4(ssample[i] * edge_kernel[i], 0.0f);
		color.a = 1.0f;
	}
	else if (confuse)
	{
		color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);
	}
	else if (shake)
	{
		for(int i = 0; i<9;i++)
			color += vec4(ssample[i] * blur_kernel[i], 0.0f);
		color.a = 1.0f;
	}
	else
	{
		color = texture(scene, TexCoords);
	}
}