#version 420 core

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

out VS_OUT {
    vec4 position;
    vec4 color;
    vec3 normal;
    vec2 texCoord;
} vs_out;

uniform mat4 projection;
uniform mat4 modelView;

void main(void)
{
    gl_Position = projection * modelView * position;

    vs_out.position = vec4((projection * modelView * position).xyz, 1);
    vs_out.color = color;
    vs_out.normal = normalize((modelView * vec4(normal.xyz, 0)).xyz);
    vs_out.texCoord = texCoord;
}
