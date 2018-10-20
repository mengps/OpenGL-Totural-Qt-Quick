#version 330 core

in vec3 color;
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texture;

void main(void)
{
    FragColor = vec4(color, 0.7) * texture2D(texture, texCoord);
}

