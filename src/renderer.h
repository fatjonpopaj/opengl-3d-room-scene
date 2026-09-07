#include "objStructs.h"
#include "mat.h"
#include "vec.h"


extern GLuint createRenderTexture(GLint width, GLint height);

extern void createFramebuffer(GLuint* framebuffer, GLuint* renderbuffer, GLuint* renderTexture, GLint width, GLint height);

extern void drawScene(objectContainer* scene, cameraObj* camera, lightObject** lights, GLuint mainProgram, GLuint shadowProgram,  GLint width, GLint height, GLuint framebuffer, GLuint texture, GLboolean drawShadows, GLfloat lightOffset);

