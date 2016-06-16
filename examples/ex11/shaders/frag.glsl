#version 120

struct DirectionalLight
{
    vec3 dir;
    vec3 color;
};

varying vec2 var_texCoord0;
varying vec3 var_normal;
varying vec3 var_tangent;
varying vec3 var_bitangent;

uniform mat4 transfMat;
uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 modelViewMat;
uniform vec3 ambientLight;
uniform DirectionalLight dirLight;

uniform sampler2D tex0;
uniform sampler2D texNormal;

void main()
{
    vec3 rawColor = texture2D(tex0, var_texCoord0).rgb;
    vec3 rawNormal = texture2D(texNormal, var_texCoord0).rgb;

    gl_FragColor = vec4(ambientLight * rawColor, 0);
    float intens = dot(dirLight.dir, var_normal);
    //if(intens < 0 ) intens = 0;
        gl_FragColor = vec4(var_normal.xyx, 0);

}
