#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

struct lightObj 
{
    vec3 pos;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};  

uniform lightObj light1;
uniform lightObj light2;

struct objMaterial
{
    vec3 emissiveColor;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};  

uniform objMaterial material;

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 c_Position;

out vec4 v_Pos;
out vec3 v_Normal;
out vec3 v_cameraDir;
out vec2 tex;
flat out vec3 v_color1;
flat out vec3 v_color2;

void main(void) 
{
    v_Pos = modelMatrix * vec4(aPos, 1.0);

    gl_Position = projectionMatrix * viewMatrix * v_Pos;

    v_Normal = normalize(normalMatrix * aNormal);
    
    v_cameraDir = normalize(c_Position - v_Pos.xyz);

    tex = texCoord;

    v_color1 = material.ambientColor * light1.ambientColor;
    v_color2 = material.ambientColor * light2.ambientColor;
}