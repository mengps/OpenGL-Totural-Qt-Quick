#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp;

attribute vec4 position;
attribute vec3 color0;
attribute vec2 texCoord0;
varying vec3 color;
varying vec2 texCoord;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp * position;
    color = color0;
    texCoord = texCoord0;
}
//! [0]
