#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp;

attribute vec4 position;
attribute vec3 color0;
varying vec3 color;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp * position;
    color = color0;
}
//! [0]
