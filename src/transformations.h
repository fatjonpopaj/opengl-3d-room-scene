#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vec.h"
#include "mat.h"

extern void lookAt(mat4* out, vec3* eye, vec3* lookAt, vec3* up);

extern void perspective(mat4* out, GLfloat yfov, GLfloat aspectRatio, GLfloat near, GLfloat far);

extern void translate(mat4* out, mat4* in, vec3* vec);

extern void scale(mat4* out, mat4* in, vec3* vec);

extern void rotatex(mat4* out, mat4* in, GLfloat angle);

extern void rotatey(mat4* out, mat4* in, GLfloat angle);

extern void rotatez(mat4* out, mat4* in, GLfloat angle);
