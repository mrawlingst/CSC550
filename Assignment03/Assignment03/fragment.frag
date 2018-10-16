#version 420 core

in VS_OUT
{
	vec3 color;
	vec3 normal;
} fs_in;

out vec4 FragColor;

void main(void)
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = vec4(fs_in.color, 1.0f);
}
