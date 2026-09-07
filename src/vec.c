#ifdef __CYGWIN__
// Wenn das Programm unter Cygwin kompiliert wird, wird die Windows-Headerdatei eingebunden
#include <windows.h>
#endif

#include <GL/glew.h> // OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h> // OpenGL Framework Library
#include <math.h> // Mathematische Funktionen

#include "vec.h" // Benutzerdefinierte Header-Datei für Vektor-Funktionen

// Funktion zum Konvertieren eines 3D-Vektors in einen 4D-Vektor
void vec3ToVec4(vec4* out, vec3* in, GLfloat w) 
{
    if (in == NULL || out == NULL)
    {
        return; // Abbrechen, wenn einer der Zeiger NULL ist
    }

    out->x = in->x; // Kopieren der x-Komponente
    out->y = in->y; // Kopieren der y-Komponente
    out->z = in->z; // Kopieren der z-Komponente
    out->w = w;     // Setzen der w-Komponente
}

// Funktion zum Konvertieren eines 4D-Vektors in einen 3D-Vektor
void vec4ToVec3(vec3* out, vec4* in) 
{
    if (in == NULL || out == NULL)
    {
        return; // Abbrechen, wenn einer der Zeiger NULL ist
    }

    out->x = in->x; // Kopieren der x-Komponente
    out->y = in->y; // Kopieren der y-Komponente
    out->z = in->z; // Kopieren der z-Komponente
}

// Funktion zum Konvertieren eines 4D-Vektors in einen homogenen 3D-Vektor
void vec4ToHomogeneousVec3(vec3* out, vec4* in) 
{
    if (in == NULL || out == NULL)
    {
        return; // Abbrechen, wenn einer der Zeiger NULL ist
    }

    out->x = in->x / in->w; // Homogenisieren der x-Komponente
    out->y = in->y / in->w; // Homogenisieren der y-Komponente
    out->z = in->z / in->w; // Homogenisieren der z-Komponente
}

// Funktion zur Berechnung der Länge eines 3D-Vektors
GLfloat getLenghtV3(vec3* vec)
{
    if (vec == NULL)
    {
        return 0; // Rückgabe 0, wenn der Zeiger NULL ist
    }

    // Berechnung der Länge des Vektors
    return sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

// Funktion zur Berechnung der Länge eines 4D-Vektors
GLfloat getLenghtV4(vec4* vec)
{
    if (vec == NULL)
    {
        return 0; // Rückgabe 0, wenn der Zeiger NULL ist
    }

    // Berechnung der Länge des Vektors
    return sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z + vec->w * vec->w);
}

// Funktion zur Normalisierung eines 3D-Vektors
void normalizeV3(vec3* vec)
{
    if (vec == NULL)
    {
        return; // Abbrechen, wenn der Zeiger NULL ist
    }

    GLfloat lenght = getLenghtV3(vec); // Berechnung der Länge des Vektors
    vec->x /= lenght; // Normalisierung der x-Komponente
    vec->y /= lenght; // Normalisierung der y-Komponente
    vec->z /= lenght; // Normalisierung der z-Komponente
}

// Funktion zur Normalisierung eines 4D-Vektors
void normalizeV4(vec4* vec)
{
    if (vec == NULL)
    {
        return; // Abbrechen, wenn der Zeiger NULL ist
    }

    GLfloat lenght = getLenghtV4(vec); // Berechnung der Länge des Vektors
    vec->x /= lenght; // Normalisierung der x-Komponente
    vec->y /= lenght; // Normalisierung der y-Komponente
    vec->z /= lenght; // Normalisierung der z-Komponente
    vec->w /= lenght; // Normalisierung der w-Komponente
}

// Funktion zur Berechnung des Skalarprodukts zweier 3D-Vektoren
GLfloat dotProductV3(vec3* a, vec3* b)
{
    if (a == NULL || b == NULL)
    {
        return 0; // Rückgabe 0, wenn einer der Zeiger NULL ist
    }

    // Berechnung des Skalarprodukts
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

// Funktion zur Berechnung des Skalarprodukts zweier 4D-Vektoren
GLfloat dotProductV4(vec4* a, vec4* b)
{
    if (a == NULL || b == NULL)
    {
        return 0; // Rückgabe 0, wenn einer der Zeiger NULL ist
    }

    // Berechnung des Skalarprodukts
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

// Funktion zur Berechnung des Kreuzprodukts zweier 3D-Vektoren
void crossProductV3(vec3* out, vec3* a, vec3* b)
{
    if (a == NULL || b == NULL || out == NULL)
    {
        return; // Abbrechen, wenn einer der Zeiger NULL ist
    }

    // Berechnung des Kreuzprodukts
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}
