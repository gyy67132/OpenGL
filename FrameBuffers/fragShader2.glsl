#version 430

out vec4 fragColor;

in vec4 varyingColor;
in vec2 outTexcoord;

uniform sampler2D texture1;

const float offset = 1.0 / 300.0;

void main()
{
	//fragColor = texture(texture1, outTexcoord);

	//反相
	//fragColor = vec4(vec3(1.0 - texture(texture1, outTexcoord)) , 1.0);
	
	//灰度
	//fragColor = texture(texture1, outTexcoord);
	//float average = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
	//fragColor = vec4(average, average, average,1.0);

	
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), 
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f,0.0f),
		vec2(offset,0.0f),
		vec2(-offset, -offset),
		vec2(0.0f,-offset),
		vec2(offset,-offset));
	//锐化
	/*float kernel[9] = float[](
		-1,-1,-1,
		-1,9,-1,
		-1,-1,-1);*/
	//模糊
	/*float kernel[9] = float[](
		1.0/16, 2.0/16, 1.0/16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16);*/
	//边缘检测
	float kernel[9] = float[](
		1,1,1,
		1,-8,1,
		1,1,1);

	vec3 sampleTex[9];

	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(texture1, outTexcoord.st + offsets[i]));
	}

	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];
	fragColor = vec4(col, 1.0);
}