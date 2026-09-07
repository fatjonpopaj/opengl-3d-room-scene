#ifdef __CYGWIN__
// Wenn das Programm unter Cygwin kompiliert wird, wird die Windows-Headerdatei eingebunden
#include <windows.h>
#endif

#ifndef M_PI
// Definition von M_PI, falls nicht bereits definiert, für den Wert von Pi
#define M_PI 3.14159265358979323846
#endif

#include <GL/glew.h> // OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h> // OpenGL Framework Library
#include <math.h> // Mathematische Funktionen

#include "mat.h" // Benutzerdefinierte Header-Datei für Matrix-Funktionen
#include "vec.h" // Benutzerdefinierte Header-Datei für Vektor-Funktionen

// Funktion zum Setzen der Look-At-Matrix
void lookAt(mat4* out, vec3* eye, vec3* lookAt, vec3* up)
{
    identityM4(out); // Setzen der Matrix auf die Einheitsmatrix
        
    vec3 xAxis, yAxis, zAxis;

    // Berechnung der Z-Achse
    zAxis.x = eye->x - lookAt->x;
    zAxis.y = eye->y - lookAt->y;
    zAxis.z = eye->z - lookAt->z;

    // Berechnung der X-Achse
    crossProductV3(&xAxis, up, &zAxis);
    normalizeV3(&xAxis);

    // Berechnung der Y-Achse
    crossProductV3(&yAxis, &zAxis, &xAxis);
    normalizeV3(&yAxis);

    // Normalisierung der Z-Achse
    crossProductV3(&zAxis, &xAxis, &yAxis);
    normalizeV3(&zAxis);

    // Setzen der Werte in die Matrix
    for (int i = 0; i < 3; i++)
    {
        out->values[i*4 + 0] = ((GLfloat*) &xAxis)[i];
        out->values[i*4 + 1] = ((GLfloat*) &yAxis)[i]; 
        out->values[i*4 + 2] = ((GLfloat*) &zAxis)[i]; 
    }

    // Berechnung der Translation
    out->values[12] = -dotProductV3(&xAxis, eye);
    out->values[13] = -dotProductV3(&yAxis, eye);
    out->values[14] = -dotProductV3(&zAxis, eye);
}

// Funktion zum Setzen der inversen Look-At-Matrix
void lookAtInverse(mat4* out, vec3* eye, vec3* lookAt, vec3* up)
{
    identityM4(out); // Setzen der Matrix auf die Einheitsmatrix
        
    vec3 xAxis, yAxis, zAxis;

    // Berechnung der Z-Achse
    zAxis.x = eye->x - lookAt->x;
    zAxis.y = eye->y - lookAt->y;
    zAxis.z = eye->z - lookAt->z;

    // Berechnung der X-Achse
    crossProductV3(&xAxis, up, &zAxis);
    normalizeV3(&xAxis);

    // Berechnung der Y-Achse
    crossProductV3(&yAxis, &zAxis, &xAxis);
    normalizeV3(&yAxis);

    // Normalisierung der Z-Achse
    crossProductV3(&zAxis, &xAxis, &yAxis);
    normalizeV3(&zAxis);

    // Setzen der Werte in die Matrix
    for (int i = 0; i < 3; i++)
    {
        out->values[i*4 + 0] = ((GLfloat*) &xAxis)[i];
        out->values[i*4 + 1] = ((GLfloat*) &yAxis)[i]; 
        out->values[i*4 + 2] = ((GLfloat*) &zAxis)[i]; 
    }

    // Berechnung der Translation
    out->values[12] = -dotProductV3(&xAxis, eye);
    out->values[13] = -dotProductV3(&yAxis, eye);
    out->values[14] = -dotProductV3(&zAxis, eye);
}

// Funktion zum Setzen der Perspektiv-Matrix
void perspective(mat4* out, GLfloat yfov, GLfloat aspectRatio, GLfloat n, GLfloat f)
{
    nullM4(out); // Setzen der Matrix auf eine Nullmatrix

    GLfloat t, l;
    t = tanf((yfov * 2.0f * M_PI / 360.0f) * 0.5f) * n; // Berechnung von t
    l = t * aspectRatio; // Berechnung von l

    out->values[0] = 1.0f / l; // Setzen des Werts für die Perspektivprojektion
    out->values[5] = 1.0f / t;
    out->values[10] = -1.0f / n * (f + n) / (f - n);
    out->values[11] = -1.0f / n;
    out->values[14] = -2.0f * f / (f - n);
}

// Funktion zum Setzen der Translations-Matrix
void translate(mat4* out, mat4* in, vec3* vec) 
{
    if (in == NULL || vec == NULL || out == NULL)
    {
        return;
    }

    mat4 resultMat[16]; // Ergebnis-Matrix

    identityM4(resultMat); // Setzen der Matrix auf die Einheitsmatrix

    // Setzen der Translationswerte
    for (int i = 0; i < 3; i++) 
    {
        resultMat->values[3*4 + i] = ((GLfloat*) vec)[i];
    }    

    multiplyM4xM4(out, resultMat, in); // Multiplizieren der Matrizen
}

// Funktion zum Setzen der Skalierungs-Matrix
void scale(mat4* out, mat4* in, vec3* vec) 
{
    if (in == NULL || vec == NULL || out == NULL)
    {
        return;
    }

    mat4 resultMat[16]; // Ergebnis-Matrix

    nullM4(resultMat); // Setzen der Matrix auf eine Nullmatrix

    // Setzen der Skalierungswerte
    for (int x = 0; x < 4; x++) 
    {
        for (int y = 0; y < 4; y++) 
        {
            if (x == 3 && y == 3) 
            {
                resultMat->values[15] = 1.0f;
                continue;
            }
            
            resultMat->values[x*4+y] = y == x ? ((GLfloat*) vec)[y] : 0.0f;
        }    
    }

    multiplyM4xM4(out, resultMat, in); // Multiplizieren der Matrizen
}

// Funktion zum Setzen der Rotations-Matrix um die X-Achse
void rotatex(mat4* out, mat4* in, GLfloat angle) 
{
    if (in == NULL || out == NULL)
    {
        return;
    }

    mat4 resultMat[16]; // Ergebnis-Matrix

    nullM4(resultMat); // Setzen der Matrix auf eine Nullmatrix
    
    // Setzen der Rotationswerte
    for (int x = 0; x < 4; x++) 
    {
        for (int y = 0; y < 4; y++) 
        {
            if (x == 3 || x == 0 || y == 3 || y == 0) {
                resultMat->values[x*4+y] = y == x ? 1.0f : 0.0f;
                continue;
            }
            
            resultMat->values[x*4+y] = x == y ? cosf(angle * 2.0f * M_PI / 360.0f) : sinf(angle * 2.0f * M_PI / 360.0f) * ((x - 1.0f) * -2.0f + 1.0f);
        }    
    }

    multiplyM4xM4(out, resultMat, in); // Multiplizieren der Matrizen
}

// Funktion zum Setzen der Rotations-Matrix um die Y-Achse
void rotatey(mat4* out, mat4* in, GLfloat angle) 
{
    if (in == NULL || out == NULL)
    {
        return;
    }

    mat4 resultMat[16]; // Ergebnis-Matrix

    nullM4(resultMat); // Setzen der Matrix auf eine Nullmatrix
    
    // Setzen der Rotationswerte
    for (int x = 0; x < 4; x++) 
    {
        for (int y = 0; y < 4; y++) 
        {
            if (x == 3 || x == 1 || y == 3 || y == 1) {
                resultMat->values[x*4+y] = y == x ? 1.0f : 0.0f;
                continue;
            }
            
            resultMat->values[x*4+y] = x == y ? cosf(angle * 2.0f * M_PI / 360.0f) : sinf(angle * 2.0f * M_PI / 360.0f) * ((x / 2.0f) * 2.0f - 1.0f);
        }    
    }

    multiplyM4xM4(out, resultMat, in); // Multiplizieren der Matrizen
}

// Funktion zum Setzen der Rotations-Matrix um die Z-Achse
void rotatez(mat4* out, mat4* in, GLfloat angle) 
{
    if (in == NULL || out == NULL)
    {
        return;
    }

    mat4 resultMat[16]; // Ergebnis-Matrix

    nullM4(resultMat); // Setzen der Matrix auf eine Nullmatrix
    
    // Setzen der Rotationswerte
    for (int x = 0; x < 4; x++) 
    {
        for (int y = 0; y < 4; y++) 
        {
            if (x >= 2 || y >= 2) {
                resultMat->values[x*4+y] = y == x ? 1.0f : 0.0f;
                continue;
            }
            
            resultMat->values[x*4+y] = x == y ? cosf(angle * 2.0f * M_PI / 360.0f) : sinf(angle * 2.0f * M_PI / 360.0f) * (x * -2.0f + 1.0f);
        }    
    }

    multiplyM4xM4(out, resultMat, in); // Multiplizieren der Matrizen
}
