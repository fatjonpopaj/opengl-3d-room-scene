#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

uniform vec3 lightPos;

out vec3 v_Pos;

void main(void) 
{
    vec4 v_Pos4 = modelMatrix * vec4(aPos, 1.0);

    gl_Position = viewProjectionMatrix * v_Pos4;

    v_Pos = v_Pos4.xyz - lightPos;
}