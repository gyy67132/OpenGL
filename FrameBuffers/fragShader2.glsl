#version 430

out vec4 fragColor;

in vec4 varyingColor;
in vec2 outTexcoord;

uniform sampler2D texture1;

void main()
{
	//∑¥œ‡
	fragColor = vec4(vec3(1.0 - texture(texture1, outTexcoord)) , 1.0);
	
	//ª“∂»
	//fragColor = texture(texture1, outTexcoord);
	//float average = 0.2126 * fragColor.r + 0.7152 * fragColor.g + 0.0722 * fragColor.b;
	//fragColor = vec4(average, average, average,1.0);

	//
}