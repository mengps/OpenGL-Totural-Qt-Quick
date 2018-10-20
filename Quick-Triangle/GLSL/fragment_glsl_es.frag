#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif

varying vec3 color;

//! [0]
void main()
{
    gl_FragColor = vec4(color, 1.0);
}
//! [0]

