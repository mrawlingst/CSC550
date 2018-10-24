#version 420 core

in VS_OUT {
    vec4 position;
    vec4 color;
    vec3 normal;
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

void main(void)
{
    FragColor = fs_in.color;
}
