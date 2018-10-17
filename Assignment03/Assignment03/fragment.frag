#version 420 core

in VS_OUT
{
	vec4 position;
	vec3 color;
	vec3 normal;
} fs_in;

out vec4 FragColor;

uniform vec3 lightPos1;
uniform vec3 lightColor1;
uniform vec3 lightPos2;
uniform vec3 lightColor2;
uniform vec3 lightPos3;
uniform vec3 lightColor3;

void main(void)
{
	vec3 lightDir1 = normalize(lightPos1.xyz - fs_in.position.xyz);
	vec3 lightDir2 = normalize(lightPos2.xyz - fs_in.position.xyz);
	vec3 lightDir3 = normalize(lightPos3.xyz - fs_in.position.xyz);

	float light1Diffuse = dot(fs_in.normal.xyz,lightDir1);
	float light2Diffuse = dot(fs_in.normal.xyz,lightDir2);
	float light3Diffuse = dot(fs_in.normal.xyz,lightDir3);

	float light1Specular = pow(dot(reflect(lightDir1, fs_in.normal.xyz), vec3(0.0,0.0,1.0)), 2.0);
	float light2Specular = pow(dot(reflect(lightDir2, fs_in.normal.xyz), vec3(0.0,0.0,1.0)), 2.0);
	float light3Specular = pow(dot(reflect(lightDir3, fs_in.normal.xyz), vec3(0.0,0.0,1.0)), 2.0);

	vec4 color = vec4(0.0);//fs_in.color;
	color.rgb += lightColor1*light1Diffuse;				
	//color.rgb += lightColor1 * light1Specular;	
	color.rgb += lightColor2*light2Diffuse;	
	//color.rgb += lightColor2 * light2Specular;	
	color.rgb += lightColor3*light3Diffuse;	
	//color.rgb += lightColor3 * light3Specular;

	FragColor = color;

    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//FragColor = vec4(fs_in.color, 1.0f);
	//FragColor = vec4(vec3(clamp(dot(fs_in.normal.xyz, normalize(vec3(0.0, 1.0, 1.0))), 0.0, 1.0)), 1.0f);
	//FragColor = vec4(fs_in.color * clamp(dot(fs_in.normal.xyz, normalize(vec3(0.0, 1.0, 1.0))), 0.0, 1.0), 1.0f);
}
