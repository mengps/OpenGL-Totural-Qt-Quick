#version 330 core

layout(location = 0) in vec4 position;
layout (location = 1) in vec3 color0;

out vec3 color;
uniform mat4 mvp;

void main(void)
{
    gl_Position = mvp * position;
    color = color0;
}
