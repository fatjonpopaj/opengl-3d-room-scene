#version 330 core

uniform float farPlane;

in vec3 v_Pos;
 
void main(void) 
{      
    gl_FragDepth = length(v_Pos) / farPlane;
}