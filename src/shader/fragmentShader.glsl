#version 330 core

#define NUM_OF_LIGHTS 2

struct lightObj 
{
    vec3 pos;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};  

struct objMaterial
{
    vec3 emissiveColor;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};  

uniform objMaterial material;
uniform lightObj light1;
uniform lightObj light2;
uniform samplerCube depthMap1;
uniform samplerCube depthMap2;
uniform float shadowFarPlane;
uniform sampler2D ourTexture;

uniform float lightOffset;

in vec4 v_Pos;
in vec3 v_Normal;
in vec3 v_cameraDir;
in vec2 tex;
flat in vec3 v_color1;
flat in vec3 v_color2;

out vec4 fragColor;

vec3 v_lightDir1;
vec3 v_lightDir2;
vec3 color1;
vec3 color2;
int inLight1;
int inLight2;
vec3 halfwayVector1;
vec3 halfwayVector2;

void main(void) 
{   
    vec3 pos_Light_dir1 = v_Pos.xyz - light1.pos;
    vec3 pos_Light_dir2 = v_Pos.xyz - light2.pos;


    color1 = v_color1;
    color2 = v_color2;

    v_lightDir1 = normalize(light1.pos - v_Pos.xyz);
    v_lightDir2 = normalize(light2.pos - v_Pos.xyz);

    float dot1 = max(dot(v_Normal, v_lightDir1), 0.0);
    float dot2 = max(dot(v_Normal, v_lightDir2), 0.0);

    halfwayVector1 = normalize(v_cameraDir + v_lightDir1);
    halfwayVector2 = normalize(v_cameraDir + v_lightDir2);

    vec3 temp1, temp2;

    temp1 = color1 * max(pow(dot1, 1.0/8.0), 0.625);
    temp1 += color1 + dot1 * material.diffuseColor * light1.diffuseColor;
    temp1 += color1 + pow(max(dot(v_Normal, halfwayVector1) * 2.0 - 1.0, 0.0), material.shininess) * material.specularColor * light1.specularColor;

    temp2 = color2 * max(pow(dot2, 1.0/8.0), 0.625);
    temp2 += color2 + dot2 * material.diffuseColor * light2.diffuseColor;
    temp2 += color2 + pow(max(dot(v_Normal, halfwayVector2) * 2.0 - 1.0, 0.0), material.shininess) * material.specularColor * light2.specularColor;

    float distance1 = length(pos_Light_dir1)/shadowFarPlane;
    float distance2 = length(pos_Light_dir2)/shadowFarPlane;

    inLight1 = texture(depthMap1, pos_Light_dir1).r + 0.005 >= distance1 ? 1 : 0;
    inLight2 = texture(depthMap2, pos_Light_dir2).r + 0.005 >= distance2 ? 1 : 0;

    color1 += (inLight1*(-1)+1) * color1 * 0.625 + inLight1 * temp1;
    color2 += (inLight2*(-1)+1) * color2 * 0.625 + inLight2 * temp2;

    color1 *= max(lightOffset+((1-distance1)*(1-distance1)), 0);
    color2 *= max(lightOffset+((1-distance2)*(1-distance2)), 0);

    //fragColor = texture(depthMap2, v_Pos.xyz - light2.pos).r * vec4(1,1,1,1);
    //fragColor = (length(v_Pos.xyz - light1.pos)/shadowFarPlane) * vec4(1,1,1,1);

    fragColor = texture(ourTexture, tex) * vec4(material.emissiveColor + color1 + color2 ,1);
}