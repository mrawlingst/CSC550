#version 420 core

in vec3 position;
in vec3 color;
in vec3 normal;

out VS_OUT
{
	vec3 color;
	vec3 normal;
} vs_out;

uniform mat4 projection;

void main(void)
{
    gl_Position = projection * vec4(position, 1);

	vs_out.color = color;
}
