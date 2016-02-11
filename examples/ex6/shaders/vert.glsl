#version 120

attribute vec2 pos;
attribute vec2 texCoord0;

varying vec2 var_texCoord0;

void main()
{
    gl_Position = vec4(pos, 0, 1);
    var_texCoord0 = texCoord0;
}
