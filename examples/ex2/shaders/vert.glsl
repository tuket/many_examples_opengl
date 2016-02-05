#version 120

attribute vec2 pos;
attribute vec3 color;

varying vec3 var_color;

void main()
{
    gl_Position = vec4(pos, 0, 1);
    var_color = color;
}
