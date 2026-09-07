#ifdef __CYGWIN__
// Wenn das Programm unter Cygwin kompiliert wird, wird die Windows-Headerdatei eingebunden
#include <windows.h>
#endif

#include <GL/glew.h> // OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h> // OpenGL Framework Library

#include <stdlib.h> // Standardbibliotheksfunktionen
#include <stdio.h> // Standard I/O Funktionen
#include <math.h> // Mathematische Funktionen

#include "mat.h" // Benutzerdefinierte Header-Datei für Matrix-Funktionen
#include "vec.h" // Benutzerdefinierte Header-Datei für Vektor-Funktionen

// Setzt eine 4x4-Matrix auf eine Nullmatrix
void nullM4(mat4* out)
{
    if (out == NULL) // Überprüfen, ob der Zeiger NULL ist
    {
        return;
    }

    for (int i = 0; i < 4; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 4; j++) // Schleife über alle Spalten
        {
            out->values[i*4 + j] = 0.0f; // Setzen des Werts auf 0.0
        }
    }
}

// Setzt eine 3x3-Matrix auf eine Nullmatrix
void nullM3(mat3* out)
{
    if (out == NULL) // Überprüfen, ob der Zeiter NULL ist
    {
        return;
    }

    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            out->values[i*3 + j] = 0.0f; // Setzen des Werts auf 0.0
        }
    }
}

// Setzt eine 4x4-Matrix auf die Einheitsmatrix
void identityM4(mat4* out)
{
    if (out == NULL) // Überprüfen, ob der Zeiter NULL ist
    {
        return;
    }

    for (int i = 0; i < 4; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 4; j++) // Schleife über alle Spalten
        {
            out->values[i*4 + j] = i == j ? 1.0f : 0.0f; // Setzen des Werts auf 1.0, wenn die Zeile gleich der Spalte ist, sonst 0.0
        }
    }
}

// Setzt eine 3x3-Matrix auf die Einheitsmatrix
void identityM3(mat3* out)
{
    if (out == NULL) // Überprüfen, ob der Zeiter NULL ist
    {
        return;
    }

    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            out->values[i*3 + j] = i == j ? 1.0f : 0.0f; // Setzen des Werts auf 1.0, wenn die Zeile gleich der Spalte ist, sonst 0.0
        }
    }
}

// Multipliziert zwei 4x4-Matrizen
void multiplyM4xM4(mat4* out, mat4* a, mat4* b)
{
    if (a == NULL || b == NULL || out == NULL) // Überprüfen, ob einer der Zeiger NULL ist
    {
        return;
    }

    mat4 resultMat; // Ergebnis-Matrix

    nullM4(&resultMat); // Setzen der Ergebnis-Matrix auf eine Nullmatrix

    for (int i = 0; i < 4; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 4; j++) // Schleife über alle Spalten
        {
            for (int k = 0; k < 4; k++) // Schleife über die gemeinsamen Dimensionen
            {
                resultMat.values[i*4 + j] += a->values[j + k*4] * (GLfloat) b->values[i*4 + k]; // Berechnen des Matrixprodukts
            }
        }
    }

    *out = resultMat; // Kopieren der Ergebnis-Matrix in die Ausgabe-Matrix
}

// Multipliziert zwei 3x3-Matrizen
void multiplyM3xM3(mat3* out, mat3* a, mat3* b)
{
    if (a == NULL || b == NULL || out == NULL) // Überprüfen, ob einer der Zeiger NULL ist
    {
        return;
    }

    mat3 resultMat; // Ergebnis-Matrix

    nullM3(&resultMat); // Setzen der Ergebnis-Matrix auf eine Nullmatrix

    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            for (int k = 0; k < 3; k++) // Schleife über die gemeinsamen Dimensionen
            {
                resultMat.values[i*3 + j] += a->values[j + k*3] * (GLfloat) b->values[i*3 + k]; // Berechnen des Matrixprodukts
            }
        }
    }

    *out = resultMat; // Kopieren der Ergebnis-Matrix in die Ausgabe-Matrix
}

// Multipliziert eine 4x4-Matrix mit einem 4D-Vektor
void multiplyM4xV4(vec4* out, mat4* a, vec4* b)
{
    if (a == NULL || b == NULL || out == NULL) // Überprüfen, ob einer der Zeiger NULL ist
    {
        return;
    }

    vec4 resultVec = {0.0f, 0.0f, 0.0f, 0.0f}; // Ergebnis-Vektor

    for (int i = 0; i < 4; i++) // Schleife über alle Dimensionen des Vektors
    {
        for (int j = 0; j < 4; j++) // Schleife über alle Dimensionen der Matrix
        {
            ((GLfloat*) &resultVec)[j] += a->values[j + i*4] * ((GLfloat*) b)[i]; // Berechnen des Matrix-Vektor-Produkts
        }
    }

    *out = resultVec; // Kopieren des Ergebnis-Vektors in den Ausgabe-Vektor
}

// Multipliziert eine 3x3-Matrix mit einem 3D-Vektor
void multiplyM3xV3(vec3* out, mat3* a, vec3* b)
{
    if (a == NULL || b == NULL || out == NULL) // Überprüfen, ob einer der Zeiter NULL ist
    {
        return;
    }

    vec3 resultVec = {0.0f, 0.0f, 0.0f}; // Ergebnis-Vektor

    for (int i = 0; i < 3; i++) // Schleife über alle Dimensionen des Vektors
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Dimensionen der Matrix
        {
            ((GLfloat*) &resultVec)[j] += a->values[j + i*3] * ((GLfloat*) b)[i]; // Berechnen des Matrix-Vektor-Produkts
        }
    }

    *out = resultVec; // Kopieren des Ergebnis-Vektors in den Ausgabe-Vektor
}

// Berechnet die Determinante einer 2x2-Matrix, die in einer 3x3-Matrix enthalten ist
GLfloat det2x2(mat3* in, int i, int j)
{
    GLfloat res; // Ergebnis

    // Indizes für die Submatrix berechnen
    GLint s1x, s1y, s2x, s2y;

    s1x = (i + 1) % 3 % 2;
    s2x = s1x + i % 2 + 1;

    s1y = (j + 1) % 3 % 2;
    s2y = s1y + j % 2 + 1;

    // Berechnung der Determinante der 2x2-Matrix
    res = in->values[s1x * 3 + s1y] * in->values[s2x * 3 + s2y] - in->values[s1x * 3 + s2y] * in->values[s2x * 3 + s1y];

    return res; // Rückgabe der Determinante
}

// Invertiert und transponiert eine 3x3-Matrix
void inverteAndTransposeM3(mat3* out, mat3* in)
{
    if (out == NULL || in == NULL) // Überprüfen, ob einer der Zeiter NULL ist
    {
        return;
    }

    // Berechnung der Determinante der Matrix
    GLfloat det = (in->values[0] * in->values[4] * in->values[8] - in->values[2] * in->values[4] * in->values[6]) +
                  (in->values[3] * in->values[7] * in->values[2] - in->values[5] * in->values[7] * in->values[0]) +
                  (in->values[6] * in->values[1] * in->values[5] - in->values[8] * in->values[1] * in->values[3]);

    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            out->values[i * 3 + j] = (((i + j + 1) % 2 * 2 - 1)) * det2x2(in, i, j) / (GLfloat)det; // Berechnung der invertierten und transponierten Matrix
        }
    }
}

// Invertiert eine 3x3-Matrix
void inverteM3(mat3* out, mat3* in)
{
    if (out == NULL || in == NULL) // Überprüfen, ob einer der Zeiter NULL ist
    {
        return;
    }

    // Berechnung der Determinante der Matrix
    GLfloat det = (in->values[0] * in->values[4] * in->values[8] - in->values[2] * in->values[4] * in->values[6]) +
                  (in->values[3] * in->values[7] * in->values[2] - in->values[5] * in->values[7] * in->values[0]) +
                  (in->values[6] * in->values[1] * in->values[5] - in->values[8] * in->values[1] * in->values[3]);

    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            out->values[i * 3 + j] = (((i + j + 1) % 2 * 2 - 1)) * det2x2(in, j, i) / (GLfloat)det; // Berechnung der invertierten Matrix
        }
    }
}

// Transponiert eine 4x4-Matrix
void transposeM4(mat4* out, mat4* in)
{
    if (out == NULL || in == NULL) // Überprüfen, ob einer der Zeiter NULL ist
    {
        return;
    }

    mat4 resultMat; // Ergebnis-Matrix

    for (int i = 0; i < 4; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 4; j++) // Schleife über alle Spalten
        {
            resultMat.values[i + j * 4] = in->values[i * 4 + j]; // Setzen des transponierten Werts
        }
    }

    *out = resultMat; // Kopieren der Ergebnis-Matrix in die Ausgabe-Matrix
}

// Transponiert eine 3x3-Matrix
void transposeM3(mat3* out, mat3* in)
{
    if (out == NULL || in == NULL) // Überprüfen, ob einer der Zeiter NULL ist
    {
        return;
    }

    mat3 resultMat; // Ergebnis-Matrix

    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            resultMat.values[i + j * 3] = in->values[i * 3 + j]; // Setzen des transponierten Werts
        }
    }

    *out = resultMat; // Kopieren der Ergebnis-Matrix in die Ausgabe-Matrix
}

// Konvertiert eine 4x4-Matrix in eine 3x3-Matrix, indem die oberen linken 3x3-Werte übernommen werden
void mat4x4ToMat3x3(mat3* out, mat4* in)
{
    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            out->values[i * 3 + j] = in->values[i * 4 + j]; // Übernehmen des Werts von der 4x4-Matrix in die 3x3-Matrix
        }
    }
}

// Gibt den Inhalt einer 4x4-Matrix auf der Konsole aus
void printM4(mat4* mat)
{
    if (mat == NULL) // Überprüfen, ob der Zeiter NULL ist
    {
        return;
    }

    printf("Mat = { "); // Start der Ausgabe

    for (int i = 0; i < 4; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 4; j++) // Schleife über alle Spalten
        {
            if (i * 4 + j == 15) // Überprüfen, ob das letzte Element erreicht ist
            {
                printf("%f ", mat->values[i * 4 + j]); // Letztes Element ohne Komma ausgeben
                continue;
            }

            printf("%f, ", mat->values[i * 4 + j]); // Element mit Komma ausgeben
        }
    }

    printf("}\n"); // Ende der Ausgabe
}

// Gibt den Inhalt einer 3x3-Matrix auf der Konsole aus
void printM3(mat3* mat)
{
    if (mat == NULL) // Überprüfen, ob der Zeiter NULL ist
    {
        return;
    }

    printf("Mat = { "); // Start der Ausgabe

    for (int i = 0; i < 3; i++) // Schleife über alle Zeilen
    {
        for (int j = 0; j < 3; j++) // Schleife über alle Spalten
        {
            if (i * 3 + j == 9) // Überprüfen, ob das letzte Element erreicht ist
            {
                printf("%f ", mat->values[i * 3 + j]); // Letztes Element ohne Komma ausgeben
                continue;
            }

            printf("%f, ", mat->values[i * 3 + j]); // Element mit Komma ausgeben
        }
    }

    printf("}\n"); // Ende der Ausgabe
}
