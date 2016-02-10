#version 120

uniform vec3 u_globColor;

attribute vec2 pos;

varying vec3 var_color;

void main()
{
    gl_Position = vec4(pos, 0, 1);
    var_color = u_globColor;
}
