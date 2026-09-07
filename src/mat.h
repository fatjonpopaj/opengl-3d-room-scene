#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vec.h"

#ifndef MATRIX3x3_STRUCT
#define MATRIX3x3_STRUCT

struct mat3
{ 
    GLfloat values[3*3];
};

typedef struct mat3 mat3;

#endif

#ifndef MATRIX4x4_STRUCT
#define MATRIX4x4_STRUCT

struct mat4
{ 
    GLfloat values[4*4];
};

typedef struct mat4 mat4;

#endif

extern void nullM4(mat4* out);

extern void nullM3(mat3* out);

extern void identityM4(mat4* out);

extern void identityM3(mat3* out);

extern void multiplyM4xM4(mat4* out, mat4* a, mat4* b);

extern void multiplyM3xM3(mat3* out, mat3* a, mat3* b);

extern void multiplyM4xV4(vec4* out, mat4* a, vec4* b);

extern void multiplyM3xV3(vec3* out, mat3* a, vec3* b);

extern void inverteAndTransposeM3(mat3* out, mat3* in);

extern void inverteM3(mat3* out, mat3* in);

extern void transposeM4(mat4* out, mat4* in);

extern void transposeM3(mat3* out, mat3* in);

extern void mat4x4ToMat3x3(mat3* out, mat4* in);

extern void printM4(mat4* mat);

extern void printM3(mat3* mat);

extern GLfloat det2x2(mat3* in, int i, int j);
