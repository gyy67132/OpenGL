#version 330

out vec4 fragColor;

in VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normals;
	vec4 FragPosLightSpace;
}fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;
uniform sampler2D floorTexture;
uniform sampler2D depthMapTexture;
uniform mat4 lightView;

void main()
{
	vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
	
	vec3 ambient = 0.05 * color;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normals);
	float diff = max(dot(lightDir, normal),0);
	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0;
	if (blinn)
	{
		vec3 halfDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
	}
	else {
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}
	vec3 specular = vec3(0.3) * spec;

	vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
	projCoords = (projCoords * 0.5 + 0.5);
	float depthA = projCoords.z;
	float depthB = texture(depthMapTexture, projCoords.xy).r;

	if(depthA > depthB + 0.005)
		fragColor = vec4(ambient, 1.0);
	else
		fragColor = vec4(ambient + diffuse + specular, 1.0);
}