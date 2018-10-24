#version 420 core

in VS_OUT {
    vec4 position;
    vec4 color;
    vec3 normal;
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture;

void main(void)
{
    //FragColor = fs_in.color;
    FragColor = texture2D(texture, fs_in.texCoord);
}
