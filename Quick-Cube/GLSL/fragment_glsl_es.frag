#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec3 color;
varying vec2 texCoord;

//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = vec4(color, 0.7) * texture2D(texture, texCoord);
}
//! [0]

