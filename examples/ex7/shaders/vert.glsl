#version 120

attribute vec2 pos;
attribute vec2 texCoord0;

varying vec2 var_texCoord0;

uniform mat4 transfMat;

void main()
{
    gl_Position = transfMat * vec4(pos, 0, 1);
    var_texCoord0 = texCoord0;
}
