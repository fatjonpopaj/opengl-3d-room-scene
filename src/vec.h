#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef VEC2_STRUCT
#define VEC2_STRUCT
struct vec2
{
    GLfloat x, y;
};
typedef struct vec2 vec2;
#endif

#ifndef VEC3_STRUCT
#define VEC3_STRUCT
struct vec3
{
    GLfloat x, y, z;
};
typedef struct vec3 vec3;
#endif

#ifndef VEC4_STRUCT
#define VEC4_STRUCT
struct vec4
{
    GLfloat x, y, z, w;
};
typedef struct vec4 vec4;
#endif

extern void vec3ToVec4(vec4* out, vec3* in, GLfloat w);

extern void vec4ToVec3(vec3* out, vec4* in);

extern void vec4ToHomogeneousVec3(vec3* out, vec4* in);

extern GLfloat getLenghtV3(vec3* vec);

extern GLfloat getLenghtV4(vec4* vec);

extern void normalizeV3(vec3* vec);

extern void normalizeV4(vec4* vec);

extern GLfloat dotProductV3(vec3* a, vec3* b);

extern GLfloat dotProductV4(vec4* a, vec4* b);

extern void crossProductV3(vec3* out, vec3* a, vec3* b);
