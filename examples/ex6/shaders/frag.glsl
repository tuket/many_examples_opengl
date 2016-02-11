#version 120

varying vec2 var_texCoord0;

uniform sampler2D texture0;

void main()
{
    gl_FragColor = texture2D(texture0, var_texCoord0);
}
