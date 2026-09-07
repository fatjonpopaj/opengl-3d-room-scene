#include <stdio.h> // Standard I/O Funktionen
#include <string.h> // Stringfunktionen

#ifdef __CYGWIN__
// Wenn das Programm unter Cygwin kompiliert wird, wird die Windows-Headerdatei eingebunden
#include <windows.h>
#endif

#include <GL/glew.h> // OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h> // OpenGL Framework Library

#include "objStructs.h" // Benutzerdefinierte Header-Datei für Objektstrukturen
#include "transformations.h" // Benutzerdefinierte Header-Datei für Transformationen
#include "mat.h" // Benutzerdefinierte Header-Datei für Matrix-Funktionen
#include "vec.h" // Benutzerdefinierte Header-Datei für Vektor-Funktionen

// Maximale Anzahl der Lichtquellen
#define MAX_LIGHT_COUNT 2

// Fern-Ebene für Schatten
#define SHADOW_FAR_PLANE 16.0

// Uniform Locations
GLuint modelLocation;
GLuint normalMatLocation;
GLuint emissiveColorLocation;
GLuint ambientColorLocation;
GLuint diffuseColorLocation;
GLuint specularColorLocation;
GLuint shininessLocation;
GLuint ourTextureLocation;
GLuint shadowModelLocation;

// Rendertexturen
GLuint renderedTexture[2] = {-1, -1};

// Funktion zur Erstellung einer Rendertextur
GLuint createRenderTexture(GLint width, GLint height)
{
    GLuint newRenderedTexture;

    glGenTextures(1, &newRenderedTexture); // Generieren einer Textur-ID

    glBindTexture(GL_TEXTURE_2D, newRenderedTexture); // Binden der Textur
  
    // Erstellen einer leeren Textur
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D); // Generieren von Mipmaps

    // Setzen der Texturparameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    glBindTexture(GL_TEXTURE_2D, 0); // Entbinden der Textur

    return newRenderedTexture; // Rückgabe der Textur-ID
}

// Funktion zur Erstellung eines Framebuffers
void createFramebuffer(GLuint* framebuffer, GLuint* renderbuffer, GLuint* renderTexture, GLint width, GLint height)
{
    // Erstellen des Framebuffers
    if (*framebuffer == 0)
    {
        glGenFramebuffers(1, framebuffer);
    }

    // Erstellen des Renderbuffers
    if (*renderbuffer == 0)
    {
        glGenRenderbuffers(1, renderbuffer);
    }
    
    // Erstellen oder Aktualisieren der Rendertextur
    if (*renderTexture == 0)
    {
        *renderTexture = createRenderTexture(width, height);
    }
    else 
    {
        glBindTexture(GL_TEXTURE_2D, *renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Binden des Framebuffers und Hinzufügen der Rendertextur und des Renderbuffers
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER,  GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *renderedTexture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, 0); // Entbinden des Renderbuffers
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Entbinden des Framebuffers
}

// Funktion zur Erstellung einer Schatten-Textur
static GLuint createShadowTexture(GLint width, GLint height)
{
    GLuint newShadowTexture;

    glGenTextures(1, &newShadowTexture); // Generieren einer Textur-ID
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, newShadowTexture); // Binden der Textur als Cube Map

    // Erstellen einer leeren Textur für jede Seite des Cube Maps
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i); // Generieren von Mipmaps
    }

    // Setzen der Texturparameter
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // Entbinden der Textur

    return newShadowTexture; // Rückgabe der Textur-ID
}

// Funktion zum Zeichnen der Schatten von Objekten
static void drawObjectShadows(renderObject* objects, GLint objCount, mat4 globalModelMat)
{
    mat4 finalModelMat, modelMat;

    for (int i = 0; i < objCount; i++) // Schleife über alle Objekte
    {
        glBindVertexArray(objects[i].vao); // Binden des Vertex-Array-Objekts

        identityM4(&modelMat); // Setzen der Matrix auf die Einheitsmatrix
        scale(&modelMat, &modelMat, &(objects[i].transform.scale)); // Skalieren der Matrix
        
        // Rotationen um die x-, y- und z-Achse
        rotatex(&modelMat, &modelMat, objects[i].transform.rotation.x);
        rotatey(&modelMat, &modelMat, objects[i].transform.rotation.y);
        rotatez(&modelMat, &modelMat, objects[i].transform.rotation.z);

        translate(&modelMat, &modelMat, &(objects[i].transform.position)); // Verschieben der Matrix

        multiplyM4xM4(&finalModelMat, &globalModelMat, &modelMat); // Multiplizieren mit der globalen Modellmatrix
        glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, finalModelMat.values); // Setzen der Modellmatrix im Shader

        glDrawElements(GL_TRIANGLES, objects[i].data.indexCount, GL_UNSIGNED_INT, NULL); // Zeichnen der Objekte

        glBindVertexArray(0); // Entbinden des Vertex-Array-Objekts
    }   
}

// Funktion zum Zeichnen der Schatten einer Unterszene
static void drawSubSceneShadows(objectContainer* scene, mat4 globalModelMat)
{
    mat4 modelMat;

    identityM4(&modelMat); // Setzen der Matrix auf die Einheitsmatrix
    scale(&modelMat, &modelMat, &scene->transform.scale); // Skalieren der Matrix
    
    // Rotationen um die x-, y- und z-Achse
    rotatex(&modelMat, &modelMat, scene->transform.rotation.x);
    rotatey(&modelMat, &modelMat, scene->transform.rotation.y);
    rotatez(&modelMat, &modelMat, scene->transform.rotation.z);

    translate(&modelMat, &modelMat, &(scene->transform.position)); // Verschieben der Matrix

    multiplyM4xM4(&globalModelMat, &globalModelMat, &modelMat); // Multiplizieren mit der globalen Modellmatrix
    drawObjectShadows(scene->objects, scene->objectCount, globalModelMat); // Zeichnen der Schatten der Objekte

    for (int i = 0; i < scene->containerCount; i++) // Schleife über alle Untercontainer
    {
        drawSubSceneShadows(&(scene->containers[i]), globalModelMat); // Zeichnen der Schatten der Untercontainer
    }
}

// Funktion zum Zeichnen der Schatten einer Szene
static void drawSceneShadow(objectContainer* scene, lightObject** lights, GLuint program, GLint width, GLint height)
{        
    GLuint viewProjectionMatrixLocation = glGetUniformLocation(program, "viewProjectionMatrix"); // Abrufen der Location der View-Projection-Matrix im Shader
    shadowModelLocation = glGetUniformLocation(program, "modelMatrix"); // Abrufen der Location der Modellmatrix im Shader

    GLuint lightPosLocation = glGetUniformLocation(program, "lightPos"); // Abrufen der Location der Lichtposition im Shader
    GLuint farPlaneLocation = glGetUniformLocation(program, "farPlane"); // Abrufen der Location der Fern-Ebene im Shader

    mat4 projectionMat;
    perspective(&projectionMat, 90.0f, width / (GLfloat)height, 0.1f, SHADOW_FAR_PLANE); // Setzen der Projektionsmatrix

    static GLuint framebuffer = -1; // Framebuffer

    glUseProgram(program); // Verwenden des Programms
    glUniform1f(farPlaneLocation, SHADOW_FAR_PLANE); // Setzen der Fern-Ebene im Shader

    mat4 globalModelMat;
    identityM4(&globalModelMat); // Setzen der globalen Modellmatrix auf die Einheitsmatrix

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Setzen der Hintergrundfarbe
    glViewport(0, 0, width, height); // Setzen des Viewports

    if (framebuffer == -1) // Wenn der Framebuffer nicht existiert, erstellen
    {
        glGenFramebuffers(1, &framebuffer);
    }

    for (int i = 0; i < MAX_LIGHT_COUNT; i++) // Schleife über alle Lichtquellen
    {
        if (renderedTexture[i] == -1) // Wenn die Rendertextur nicht existiert, erstellen
        {
            renderedTexture[i] = createShadowTexture(width, height);
        }
        else 
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, renderedTexture[i]);
            for (unsigned int j = 0; j < 6; j++) // Aktualisieren der Texturen für jede Seite des Cube Maps
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // Binden des Framebuffers
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderedTexture[i], 0); // Hinzufügen der Rendertextur als Tiefenanhang
        glDrawBuffer(GL_NONE); // Keine Farb-Buffer verwenden
        glReadBuffer(GL_NONE); // Keine Farb-Buffer lesen

        glUniform3f(lightPosLocation, lights[i]->position.x, lights[i]->position.y, lights[i]->position.z); // Setzen der Lichtposition im Shader

        mat4 viewProjectionMat;

        for (int j = 0; j < 6; j++) // Schleife über alle Seiten des Cube Maps
        {
            GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + j;
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, renderedTexture[i], 0); // Hinzufügen der Textur als Tiefenanhang

            glClear(GL_DEPTH_BUFFER_BIT); // Tiefen-Buffer löschen

            vec3 lookAtVec = (vec3){lights[i]->position.x + ((j % 2) * (-2) + 1) * (((j / 2) + 1) % 3 % 2), lights[i]->position.y + ((j % 2) * -2 + 1) * (j / 2) % 2, lights[i]->position.z + ((j % 2) * -2 + 1) * (j / 4)};

            lookAt(&viewProjectionMat, &(lights[i]->position), &lookAtVec, &((vec3){0, -1 * ((j / 2) + 1) % 2, ((j % 2) * -2 + 1) * (j / 2) % 2})); // Berechnen der View-Matrix
            multiplyM4xM4(&viewProjectionMat, &projectionMat, &viewProjectionMat); // Multiplizieren mit der Projektionsmatrix

            glUniformMatrix4fv(viewProjectionMatrixLocation, 1, GL_FALSE, viewProjectionMat.values); // Setzen der View-Projection-Matrix im Shader
                    
            drawSubSceneShadows(scene, globalModelMat); // Zeichnen der Schatten der Unterszene
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Entbinden des Framebuffers
    }
}

// Funktion zum Zeichnen der Objekte
static void drawObjects(renderObject* objects, GLint objCount, cameraObj* camera, mat4 globalModelMat)
{
    mat4 finalModelMat, modelMat;
    mat3 normalMat;

    for (int i = 0; i < objCount; i++) // Schleife über alle Objekte
    {
        glBindTexture(GL_TEXTURE_2D, objects[i].data.material.texture); // Binden der Textur des Objekts

        glBindVertexArray(objects[i].vao); // Binden des Vertex-Array-Objekts

        identityM4(&modelMat); // Setzen der Matrix auf die Einheitsmatrix
        scale(&modelMat, &modelMat, &(objects[i].transform.scale)); // Skalieren der Matrix
        
        // Rotationen um die x-, y- und z-Achse
        rotatex(&modelMat, &modelMat, objects[i].transform.rotation.x);
        rotatey(&modelMat, &modelMat, objects[i].transform.rotation.y);
        rotatez(&modelMat, &modelMat, objects[i].transform.rotation.z);

        translate(&modelMat, &modelMat, &(objects[i].transform.position)); // Verschieben der Matrix

        multiplyM4xM4(&finalModelMat, &globalModelMat, &modelMat); // Multiplizieren mit der globalen Modellmatrix
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, finalModelMat.values); // Setzen der Modellmatrix im Shader

        mat4x4ToMat3x3(&normalMat, &finalModelMat); // Umwandeln der 4x4-Matrix in eine 3x3-Matrix
        inverteAndTransposeM3(&normalMat, &normalMat); // Invertieren und Transponieren der Matrix
        glUniformMatrix3fv(normalMatLocation, 1, GL_FALSE, normalMat.values); // Setzen der Normalmatrix im Shader

        objMaterial material = objects[i].data.material;

        // Setzen der Materialeigenschaften im Shader
        glUniform3f(emissiveColorLocation, material.emissiveColor.x, material.emissiveColor.y, material.emissiveColor.z);
        glUniform3f(ambientColorLocation, material.ambientColor.x, material.ambientColor.y, material.ambientColor.z);
        glUniform3f(diffuseColorLocation, material.diffuseColor.x, material.diffuseColor.y, material.diffuseColor.z);
        glUniform3f(specularColorLocation, material.specularColor.x, material.specularColor.y, material.specularColor.z);
        glUniform1f(shininessLocation, material.shininess);

        glDrawElements(GL_TRIANGLES, objects[i].data.indexCount, GL_UNSIGNED_INT, NULL); // Zeichnen der Objekte

        glBindVertexArray(0); // Entbinden des Vertex-Array-Objekts
        glBindTexture(GL_TEXTURE_2D, 0); // Entbinden der Textur
    }
}

// Funktion zum Zeichnen einer Unterszene
static void drawSubScene(objectContainer* scene, cameraObj* camera, mat4 globalModelMat)
{
    mat4 modelMat;

    identityM4(&modelMat); // Setzen der Matrix auf die Einheitsmatrix
    scale(&modelMat, &modelMat, &scene->transform.scale); // Skalieren der Matrix
    
    // Rotationen um die x-, y- und z-Achse
    rotatex(&modelMat, &modelMat, scene->transform.rotation.x);
    rotatey(&modelMat, &modelMat, scene->transform.rotation.y);
    rotatez(&modelMat, &modelMat, scene->transform.rotation.z);

    translate(&modelMat, &modelMat, &(scene->transform.position)); // Verschieben der Matrix

    multiplyM4xM4(&globalModelMat, &globalModelMat, &modelMat); // Multiplizieren mit der globalen Modellmatrix
    
    drawObjects(scene->objects, scene->objectCount, camera, globalModelMat); // Zeichnen der Objekte

    for (int i = 0; i < scene->containerCount; i++) // Schleife über alle Untercontainer
    {
        drawSubScene(&(scene->containers[i]), camera, globalModelMat); // Zeichnen der Untercontainer
    }
}

// Funktion zum Zeichnen der Szene
void drawScene(objectContainer* scene, cameraObj* camera, lightObject** lights, GLuint mainProgram, GLuint shadowProgram, GLint width, GLint height, GLuint framebuffer, GLuint texture, GLboolean drawShadows, GLfloat lightOffset)
{
    if (drawShadows == GL_TRUE) // Wenn Schatten gezeichnet werden sollen
    {
        drawSceneShadow(scene, lights, shadowProgram, 1024, 1024); // Zeichnen der Schatten
    }

    if (framebuffer != 0) // Wenn ein Framebuffer angegeben ist
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // Binden des Framebuffers
        glFramebufferTexture2D(GL_FRAMEBUFFER,  GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0); // Hinzufügen der Textur als Farbanhang
    }

    mat4 viewMat, projectionMat;

    // Berechnen des Blickvektors
    vec3 lookAtVec = (vec3){camera->transform.position.x + camera->transform.rotation.x, camera->transform.position.y + camera->transform.rotation.y, camera->transform.position.z + camera->transform.rotation.z};

    lookAt(&viewMat, &camera->transform.position, &lookAtVec, &camera->up); // Berechnen der View-Matrix
    perspective(&projectionMat, camera->fov, camera->aspectRatio, camera->nearPlane, camera->farPlane); // Berechnen der Projektionsmatrix

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // Setzen der Hintergrundfarbe
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Löschen der Farb- und Tiefen-Buffer
    glViewport(0, 0, width, height); // Setzen des Viewports

    glUseProgram(mainProgram); // Verwenden des Hauptprogramms

    // Abrufen der Locations der Matrizen im Shader
    modelLocation = glGetUniformLocation(mainProgram, "modelMatrix");
    normalMatLocation = glGetUniformLocation(mainProgram, "normalMatrix");
    GLuint viewLocation = glGetUniformLocation(mainProgram, "viewMatrix");
    GLuint projectionLocation = glGetUniformLocation(mainProgram, "projectionMatrix");

    // Abrufen der Locations der Materialeigenschaften im Shader
    emissiveColorLocation = glGetUniformLocation(mainProgram, "material.emissiveColor");
    ambientColorLocation = glGetUniformLocation(mainProgram, "material.ambientColor");
    diffuseColorLocation = glGetUniformLocation(mainProgram, "material.diffuseColor");
    specularColorLocation = glGetUniformLocation(mainProgram, "material.specularColor");
    shininessLocation = glGetUniformLocation(mainProgram, "material.shininess");

    GLuint shadowFarPlaneLocation = glGetUniformLocation(mainProgram, "shadowFarPlane");

    glUniform1f(shadowFarPlaneLocation, SHADOW_FAR_PLANE); // Setzen der Fern-Ebene im Shader


    GLuint lightOffsetLocation = glGetUniformLocation(mainProgram, "lightOffset");

    glUniform1f(lightOffsetLocation, lightOffset); // Setzen der Fern-Ebene im Shader

    for (int i = 0; i < MAX_LIGHT_COUNT; i++) // Schleife über alle Lichtquellen
    {
        char base[16], final[32];
        snprintf(base, 16, "light%d", i+1);

        // Abrufen und Setzen der Locations der Lichtposition und -farben im Shader
        strcpy(final, base);
        strcat(final, ".pos");
        GLuint lightPosLocation = glGetUniformLocation(mainProgram, final);
        strcpy(final, base);
        strcat(final, ".ambientColor");
        GLuint lightAmbientColorLocation = glGetUniformLocation(mainProgram, final);
        strcpy(final, base);
        strcat(final, ".diffuseColor");
        GLuint lightDiffuseColorLocation = glGetUniformLocation(mainProgram, final);
        strcpy(final, base);
        strcat(final, ".specularColor");
        GLuint lightSpecularColorLocation = glGetUniformLocation(mainProgram, final);

        glUniform3f(lightPosLocation, lights[i]->position.x, lights[i]->position.y, lights[i]->position.z);
        glUniform3f(lightAmbientColorLocation, lights[i]->lightMaterial.ambientColor.x, lights[i]->lightMaterial.ambientColor.y, lights[i]->lightMaterial.ambientColor.z);
        glUniform3f(lightDiffuseColorLocation, lights[i]->lightMaterial.diffuseColor.x, lights[i]->lightMaterial.diffuseColor.y, lights[i]->lightMaterial.diffuseColor.z);
        glUniform3f(lightSpecularColorLocation, lights[i]->lightMaterial.specularColor.x, lights[i]->lightMaterial.specularColor.y, lights[i]->lightMaterial.specularColor.z);

        snprintf(base, 16, "depthMap%d", i+1);

        GLuint depthMapLocation = glGetUniformLocation(mainProgram, base);

        glUniform1i(depthMapLocation, i + 1); // Setzen der Tiefenkarte im Shader

        glActiveTexture(GL_TEXTURE0 + i + 1); // Aktivieren der Textur-Einheit
        glBindTexture(GL_TEXTURE_CUBE_MAP, renderedTexture[i]); // Binden der Cube Map Textur
    }

    // Abrufen der Location der Kameraposition im Shader
    GLuint c_PositionLocation = glGetUniformLocation(mainProgram, "c_Position");

    ourTextureLocation = glGetUniformLocation(mainProgram, "ourTexture");

    // Setzen der View- und Projektionsmatrizen im Shader
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMat.values);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMat.values);

    // Setzen der Kameraposition im Shader
    glUniform3f(c_PositionLocation, camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);

    glUniform1i(ourTextureLocation, 0);
    glActiveTexture(GL_TEXTURE0);        

    mat4 globalModelMat;
    identityM4(&globalModelMat); // Setzen der globalen Modellmatrix auf die Einheitsmatrix

    drawSubScene(scene, camera, globalModelMat); // Zeichnen der Unterszene

    if (framebuffer != 0) // Wenn ein Framebuffer angegeben ist
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Entbinden des Framebuffers
    }
}
