#version 420 core

in vec3 position;
in vec3 color;
in vec3 normal;

out VS_OUT
{
	vec4 position;
	vec3 color;
	vec3 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 modelView;

void main(void)
{
    gl_Position = projection * vec4(position, 1);

	vs_out.position = modelView * vec4(position, 1);
	vs_out.color = color;
	vs_out.normal = normalize((modelView * vec4(normal.xyz, 0.0f)).xyz);
}
