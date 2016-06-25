#version 120

struct DirectionalLight
{
    vec3 dir;
    vec3 color;
};

attribute vec3 pos;
attribute vec2 texCoord0;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 bitangent;

varying vec2 var_texCoord0;
varying vec3 var_normal;
varying vec3 var_tangent;
varying vec3 var_bitangent;

uniform mat4 transfMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

uniform vec3 ambientLight;
uniform DirectionalLight dirLight;

void main()
{
    gl_Position = transfMat * modelMat * vec4(pos, 1);
    var_texCoord0 = texCoord0;

    vec4 normal4 = modelMat * vec4(normal, 1);
    vec4 tangent4 = modelMat * vec4(tangent, 1);
    vec4 bitangent4 = modelMat * vec4(bitangent, 1);
    var_normal = normalize(normal4.xyz);
    var_tangent = normalize(tangent4.xyz);
    var_bitangent = normalize(bitangent4.xyz);
}
