#version 420 core

in vec3 position;

uniform mat4 projection;

void main(void)
{
    gl_Position = projection * vec4(position, 1);
}
