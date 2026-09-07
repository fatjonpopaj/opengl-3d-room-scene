#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vec.h"
#include "mat.h"

#ifndef VERTEX_STRUCT
#define VERTEX_STRUCT
struct vertex
{
    vec3 position;
    vec3 normal;
    vec2 textureCoord;
};

typedef struct vertex vertex;
#endif

#ifndef OBJMATERIAL_STRUCT
#define OBJMATERIAL_STRUCT
struct objMaterial
{
    vec3 emissiveColor;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    GLfloat shininess;
    GLuint texture;
};

typedef struct objMaterial objMaterial;
#endif

#ifndef LIGHTMATERIAL_STRUCT
#define LIGHTMATERIAL_STRUCT
struct lightMaterial
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};

typedef struct lightMaterial lightMaterial;
#endif

#ifndef GAMEOBJECT_DATA_STRUCT
#define GAMEOBJECT_DATA_STRUCT
struct gameObjectData
{
    vertex* vertecies;
    GLuint* indicies;
    GLint vertexCount;
    GLint indexCount;
    objMaterial material;
};

typedef struct gameObjectData gameObjectData;
#endif

#ifndef GAMEOBJECT_STRUCT
#define GAMEOBJECT_STRUCT
struct gameObject
{
    vec3 position;
    vec3 scale;
    vec3 rotation;
};

typedef struct gameObject gameObject;
#endif

#ifndef RENDEROBJECT_STRUCT
#define RENDEROBJECT_STRUCT
struct renderObject
{
    gameObject transform;
    gameObjectData data;
    GLuint vao;
    //mat4 modelMat;
};

typedef struct renderObject renderObject;
#endif

#ifndef LIGHTOBJECT_STRUCT
#define LIGHTOBJECT_STRUCT
struct lightObject
{
    vec3 position;
    lightMaterial lightMaterial;
};

typedef struct lightObject lightObject;
#endif


#ifndef CONTAINER_STRUCT
#define CONTAINER_STRUCT

struct objectContainer;

typedef struct objectContainer objectContainer;

struct objectContainer
{
    objectContainer* containers;
    renderObject* objects;
    GLint containerCount;
    GLint objectCount;
    gameObject transform;
};
#endif

#ifndef CAMERA_OBJECT_STRUCT
#define CAMERA_OBJECT_STRUCT

struct cameraObj
{
    gameObject transform;
    vec3 up;
    GLfloat aspectRatio;
    GLfloat nearPlane;
    GLfloat farPlane;
    GLfloat fov;

};

typedef struct cameraObj cameraObj;

#endif

