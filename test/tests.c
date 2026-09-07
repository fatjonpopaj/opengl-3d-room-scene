// tests.c

#ifdef __CYGWIN__
#include <windows.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "mat.h"
#include "vec.h"
#include "transformations.h"



//mat.c funktionen

void testNullM4(void) {
    mat4 matrix;
    nullM4(&matrix);

    // Erwartetes Ergebnis: alle Werte sind 0.0f
    bool isCorrect = true;
    for (int i = 0; i < 16; i++) {
        if (matrix.values[i] != 0.0f) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Matrix (4x4) wurde auf null gesetzt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Matrix (4x4) wurde nicht auf null gesetzt.\n");
    }

    // Ausgabe der Matrix zur Verifizierung
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", matrix.values[i * 4 + j]);
        }
        printf("\n");
    }
}

void testNullM3(void) {
    mat3 matrix;
    nullM3(&matrix);

    // Erwartetes Ergebnis: alle Werte sind 0.0f
    bool isCorrect = true;
    for (int i = 0; i < 9; i++) {
        if (matrix.values[i] != 0.0f) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Matrix (3x3) wurde auf null gesetzt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Matrix (3x3) wurde nicht auf null gesetzt.\n");
    }

    // Ausgabe der Matrix zur Verifizierung
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", matrix.values[i * 3 + j]);
        }
        printf("\n");
    }
}

void testIdentityM4(void) {
    mat4 matrix;
    identityM4(&matrix);

    // Erwartetes Ergebnis: Einheitsmatrix
    bool isCorrect = true;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (matrix.values[i * 4 + j] != (i == j ? 1.0f : 0.0f)) {
                isCorrect = false;
                break;
            }
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Einheitsmatrix (4x4) wurde korrekt gesetzt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Einheitsmatrix (4x4) wurde nicht korrekt gesetzt.\n");
    }

    // Ausgabe der Matrix zur Verifizierung
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", matrix.values[i * 4 + j]);
        }
        printf("\n");
    }
}

void testIdentityM3(void) {
    mat3 matrix;
    identityM3(&matrix);

    // Erwartetes Ergebnis: Einheitsmatrix
    bool isCorrect = true;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix.values[i * 3 + j] != (i == j ? 1.0f : 0.0f)) {
                isCorrect = false;
                break;
            }
        }
        if (!isCorrect) break;
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Einheitsmatrix (3x3) wurde korrekt gesetzt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Einheitsmatrix (3x3) wurde nicht korrekt gesetzt.\n");
    }

    // Ausgabe der Matrix zur Verifizierung
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", matrix.values[i * 3 + j]);
        }
        printf("\n");
    }
}

void testMultiplyM4xM4(void) {
    mat4 a = {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};

    mat4 b = {{
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    }};

    mat4 expected = {{
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    }};

    mat4 result;
    multiplyM4xM4(&result, &a, &b);

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 16; i++) {
        if (result.values[i] != expected.values[i]) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Matrixmultiplikation (4x4) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Matrixmultiplikation (4x4) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", result.values[i * 4 + j]);
        }
        printf("\n");
    }
}

void testMultiplyM3xM3(void) {
    mat3 a = {{
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    }};

    mat3 b = {{
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    }};

    mat3 expected = {{
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    }};

    mat3 result;
    multiplyM3xM3(&result, &a, &b);

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 9; i++) {
        if (result.values[i] != expected.values[i]) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Matrixmultiplikation (3x3) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Matrixmultiplikation (3x3) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", result.values[i * 3 + j]);
        }
        printf("\n");
    }
}

void testMultiplyM4xV4(void) {
    mat4 matrix = {{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};

    vec4 vector = {1.0f, 2.0f, 3.0f, 4.0f};
    vec4 expected = {1.0f, 2.0f, 3.0f, 4.0f}; // Da die Matrix die Identitätsmatrix ist, sollte das Ergebnis gleich dem Vektor sein

    vec4 result;
    multiplyM4xV4(&result, &matrix, &vector);

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = true;
    for (int i = 0; i < 4; i++) {
        if (fabs(((GLfloat*)&result)[i] - ((GLfloat*)&expected)[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Matrix-Vektor-Multiplikation (4x4 * 4x1) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Matrix-Vektor-Multiplikation (4x4 * 4x1) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    for (int i = 0; i < 4; i++) {
        printf("%f ", ((GLfloat*)&result)[i]);
    }
    printf("\n");
}

void testMultiplyM3xV3(void) {
    mat3 matrix = {{
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    }};

    vec3 vector = {1.0f, 2.0f, 3.0f};
    vec3 expected = {1.0f, 2.0f, 3.0f}; // Da die Matrix die Identitätsmatrix ist, sollte das Ergebnis gleich dem Vektor sein

    vec3 result;
    multiplyM3xV3(&result, &matrix, &vector);

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = true;
    for (int i = 0; i < 3; i++) {
        if (fabs(((GLfloat*)&result)[i] - ((GLfloat*)&expected)[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Matrix-Vektor-Multiplikation (3x3 * 3x1) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Matrix-Vektor-Multiplikation (3x3 * 3x1) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    for (int i = 0; i < 3; i++) {
        printf("%f ", ((GLfloat*)&result)[i]);
    }
    printf("\n");
}

void testDet2x2(void) {
    mat3 matrix = {{
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    }};

    // Testfall 1: Bestimmen der 2x2 Determinante aus der oberen linken Ecke
    GLfloat expected1 = 5.0f * 9.0f - 6.0f * 8.0f; // Submatrix [[5, 6], [8, 9]]
    GLfloat result1 = det2x2(&matrix, 0, 0);
    
    if (result1 == expected1) {
        printf("Test erfolgreich: Det2x2 (0, 0) ist korrekt.\n");
    } else {
        printf("Test fehlgeschlagen: Det2x2 (0, 0) ist nicht korrekt.\n");
    }

    // Testfall 2: Bestimmen der 2x2 Determinante aus der Mitte
    GLfloat expected2 = 1.0f * 9.0f - 3.0f * 7.0f; // Submatrix [[1, 3], [7, 9]]
    GLfloat result2 = det2x2(&matrix, 1, 1);
    
    if (result2 == expected2) {
        printf("Test erfolgreich: Det2x2 (1, 1) ist korrekt.\n");
    } else {
        printf("Test fehlgeschlagen: Det2x2 (1, 1) ist nicht korrekt.\n");
    }

    // Testfall 3: Bestimmen der 2x2 Determinante aus der unteren rechten Ecke
    GLfloat expected3 = 1.0f * 5.0f - 2.0f * 4.0f; // Submatrix [[1, 2], [4, 5]]
    GLfloat result3 = det2x2(&matrix, 2, 2);
    
    if (result3 == expected3) {
        printf("Test erfolgreich: Det2x2 (2, 2) ist korrekt.\n");
    } else {
        printf("Test fehlgeschlagen: Det2x2 (2, 2) ist nicht korrekt.\n");
    }
}

void testInverteAndTransposeM3(void) {
    mat3 in = {{
        1.0f, 2.0f, 3.0f,
        0.0f, 1.0f, 4.0f,
        5.0f, 6.0f, 0.0f
    }};

    mat3 expected = {{
        -24.0f, 20.0f, -5.0f,
        18.0f, -15.0f, 4.0f,
        5.0f, -4.0f, 1.0f
    }};

    mat3 result;
    inverteAndTransposeM3(&result, &in);

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 9; i++) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Invertierung und Transponierung (3x3) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Invertierung und Transponierung (3x3) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", result.values[i * 3 + j]);
        }
        printf("\n");
    }
}


void testInverteM3(void) {
    mat3 in = {{
        1.0f, 2.0f, 3.0f,
        0.0f, 1.0f, 4.0f,
        5.0f, 6.0f, 0.0f
    }};

    mat3 expected = {{
        -24.0f, 18.0f, 5.0f,
        20.0f, -15.0f, -4.0f,
        -5.0f, 4.0f, 1.0f
    }};

    mat3 result;
    inverteM3(&result, &in);

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 9; i++) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Invertierung (3x3) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Invertierung (3x3) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", result.values[i * 3 + j]);
        }
        printf("\n");
    }
}

void testTransposeM4(void) {
    mat4 in = {{
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    }};

    mat4 expected = {{
        1.0f, 5.0f, 9.0f, 13.0f,
        2.0f, 6.0f, 10.0f, 14.0f,
        3.0f, 7.0f, 11.0f, 15.0f,
        4.0f, 8.0f, 12.0f, 16.0f
    }};

    mat4 result;
    transposeM4(&result, &in);

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 16; i++) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Transponierung (4x4) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Transponierung (4x4) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", result.values[i * 4 + j]);
        }
        printf("\n");
    }
}

void testTransposeM3(void) {
    mat3 in = {{
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    }};

    mat3 expected = {{
        1.0f, 4.0f, 7.0f,
        2.0f, 5.0f, 8.0f,
        3.0f, 6.0f, 9.0f
    }};

    mat3 result;
    transposeM3(&result, &in);

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 9; i++) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Transponierung (3x3) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Transponierung (3x3) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", result.values[i * 3 + j]);
        }
        printf("\n");
    }
}

void testMat4x4ToMat3x3(void) {
    mat4 in = {{
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    }};

    mat3 expected = {{
        1.0f, 2.0f, 3.0f,
        5.0f, 6.0f, 7.0f,
        9.0f, 10.0f, 11.0f
    }};

    mat3 result;
    mat4x4ToMat3x3(&result, &in);

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 9; i++) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: Die Umwandlung (4x4 zu 3x3) wurde korrekt durchgeführt.\n");
    } else {
        printf("Test fehlgeschlagen: Die Umwandlung (4x4 zu 3x3) wurde nicht korrekt durchgeführt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", result.values[i * 3 + j]);
        }
        printf("\n");
    }
}

void testPrintM4(void) {
    mat4 matrix = {{
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    }};

    // Erwartete Ausgabe: "Mat = { 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000, 14.000000, 15.000000, 16.000000 }"

    printf("Test für printM4:\n");
    printM4(&matrix);
}

void testPrintM3(void) {
    mat3 matrix = {{
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    }};

    // Erwartete Ausgabe: "Mat = { 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000 }"

    printf("Test für printM3:\n");
    printM3(&matrix);
}

// vec.c funktionen

void testVec3ToVec4(void) {
    vec3 in = {1.0f, 2.0f, 3.0f};
    GLfloat w = 4.0f;
    vec4 result;
    vec3ToVec4(&result, &in, w);

    vec4 expected = {1.0f, 2.0f, 3.0f, 4.0f};

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = true;
    if (fabs(result.x - expected.x) > 1e-5 ||
        fabs(result.y - expected.y) > 1e-5 ||
        fabs(result.z - expected.z) > 1e-5 ||
        fabs(result.w - expected.w) > 1e-5) {
        isCorrect = false;
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: vec3ToVec4 hat den 3D-Vektor korrekt in einen 4D-Vektor umgewandelt.\n");
    } else {
        printf("Test fehlgeschlagen: vec3ToVec4 hat den 3D-Vektor nicht korrekt in einen 4D-Vektor umgewandelt.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    printf("%f %f %f %f\n", result.x, result.y, result.z, result.w);
}

void testVec4ToVec3(void) {
    vec4 in = {1.0f, 2.0f, 3.0f, 4.0f};
    vec3 result;
    vec4ToVec3(&result, &in);

    vec3 expected = {1.0f, 2.0f, 3.0f};

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = true;
    if (fabs(result.x - expected.x) > 1e-5 ||
        fabs(result.y - expected.y) > 1e-5 ||
        fabs(result.z - expected.z) > 1e-5) {
        isCorrect = false;
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: vec4ToVec3 hat den 4D-Vektor korrekt in einen 3D-Vektor umgewandelt.\n");
    } else {
        printf("Test fehlgeschlagen: vec4ToVec3 hat den 4D-Vektor nicht korrekt in einen 3D-Vektor umgewandelt.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    printf("%f %f %f\n", result.x, result.y, result.z);
}

void testVec4ToHomogeneousVec3(void) {
    vec4 in = {2.0f, 4.0f, 6.0f, 2.0f};
    vec3 result;
    vec4ToHomogeneousVec3(&result, &in);

    vec3 expected = {1.0f, 2.0f, 3.0f};

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = true;
    if (fabs(result.x - expected.x) > 1e-5 ||
        fabs(result.y - expected.y) > 1e-5 ||
        fabs(result.z - expected.z) > 1e-5) {
        isCorrect = false;
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: vec4ToHomogeneousVec3 hat den 4D-Vektor korrekt in einen 3D-homogenen Vektor umgewandelt.\n");
    } else {
        printf("Test fehlgeschlagen: vec4ToHomogeneousVec3 hat den 4D-Vektor nicht korrekt in einen 3D-homogenen Vektor umgewandelt.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    printf("%f %f %f\n", result.x, result.y, result.z);
}

void testGetLenghtV3(void) {
    vec3 vec = {3.0f, 4.0f, 0.0f};
    GLfloat result = getLenghtV3(&vec);

    GLfloat expected = 5.0f;

    // Überprüfen, ob das Ergebnis dem erwarteten Wert entspricht
    bool isCorrect = fabs(result - expected) < 1e-5;

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: getLenghtV3 hat die Länge des 3D-Vektors korrekt berechnet.\n");
    } else {
        printf("Test fehlgeschlagen: getLenghtV3 hat die Länge des 3D-Vektors nicht korrekt berechnet.\n");
    }

    // Ausgabe des Ergebnis-Wertes zur Verifizierung
    printf("Ergebnis-Wert: %f\n", result);
}

void testGetLenghtV4(void) {
    vec4 vec = {1.0f, 2.0f, 3.0f, 4.0f};
    GLfloat result = getLenghtV4(&vec);

    GLfloat expected = sqrtf(1.0f * 1.0f + 2.0f * 2.0f + 3.0f * 3.0f + 4.0f * 4.0f);

    // Überprüfen, ob das Ergebnis dem erwarteten Wert entspricht
    bool isCorrect = fabs(result - expected) < 1e-5;

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: getLenghtV4 hat die Länge des 4D-Vektors korrekt berechnet.\n");
    } else {
        printf("Test fehlgeschlagen: getLenghtV4 hat die Länge des 4D-Vektors nicht korrekt berechnet.\n");
    }

    // Ausgabe des Ergebnis-Wertes zur Verifizierung
    printf("Ergebnis-Wert: %f\n", result);
}

void testNormalizeV3(void) {
    vec3 vec = {3.0f, 4.0f, 0.0f};
    normalizeV3(&vec);

    vec3 expected = {0.6f, 0.8f, 0.0f}; // Erwarteter normalisierter Vektor

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = fabs(vec.x - expected.x) < 1e-5 &&
                     fabs(vec.y - expected.y) < 1e-5 &&
                     fabs(vec.z - expected.z) < 1e-5;

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: normalizeV3 hat den 3D-Vektor korrekt normalisiert.\n");
    } else {
        printf("Test fehlgeschlagen: normalizeV3 hat den 3D-Vektor nicht korrekt normalisiert.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    printf("%f %f %f\n", vec.x, vec.y, vec.z);
}

void testNormalizeV4(void) {
    vec4 vec = {1.0f, 2.0f, 3.0f, 4.0f};
    normalizeV4(&vec);

    GLfloat length = sqrtf(1.0f * 1.0f + 2.0f * 2.0f + 3.0f * 3.0f + 4.0f * 4.0f);
    vec4 expected = {1.0f / length, 2.0f / length, 3.0f / length, 4.0f / length}; // Erwarteter normalisierter Vektor

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = fabs(vec.x - expected.x) < 1e-5 &&
                     fabs(vec.y - expected.y) < 1e-5 &&
                     fabs(vec.z - expected.z) < 1e-5 &&
                     fabs(vec.w - expected.w) < 1e-5;

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: normalizeV4 hat den 4D-Vektor korrekt normalisiert.\n");
    } else {
        printf("Test fehlgeschlagen: normalizeV4 hat den 4D-Vektor nicht korrekt normalisiert.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    printf("%f %f %f %f\n", vec.x, vec.y, vec.z, vec.w);
}

void testDotProductV3(void) {
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 b = {4.0f, 5.0f, 6.0f};
    GLfloat result = dotProductV3(&a, &b);

    GLfloat expected = 1.0f * 4.0f + 2.0f * 5.0f + 3.0f * 6.0f; // Erwartetes Skalarprodukt

    // Überprüfen, ob das Ergebnis dem erwarteten Wert entspricht
    bool isCorrect = fabs(result - expected) < 1e-5;

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: dotProductV3 hat das Skalarprodukt der 3D-Vektoren korrekt berechnet.\n");
    } else {
        printf("Test fehlgeschlagen: dotProductV3 hat das Skalarprodukt der 3D-Vektoren nicht korrekt berechnet.\n");
    }

    // Ausgabe des Ergebnis-Wertes zur Verifizierung
    printf("Ergebnis-Wert: %f\n", result);
}

void testDotProductV4(void) {
    vec4 a = {1.0f, 2.0f, 3.0f, 4.0f};
    vec4 b = {5.0f, 6.0f, 7.0f, 8.0f};
    GLfloat result = dotProductV4(&a, &b);

    GLfloat expected = 1.0f * 5.0f + 2.0f * 6.0f + 3.0f * 7.0f + 4.0f * 8.0f; // Erwartetes Skalarprodukt

    // Überprüfen, ob das Ergebnis dem erwarteten Wert entspricht
    bool isCorrect = fabs(result - expected) < 1e-5;

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: dotProductV4 hat das Skalarprodukt der 4D-Vektoren korrekt berechnet.\n");
    } else {
        printf("Test fehlgeschlagen: dotProductV4 hat das Skalarprodukt der 4D-Vektoren nicht korrekt berechnet.\n");
    }

    // Ausgabe des Ergebnis-Wertes zur Verifizierung
    printf("Ergebnis-Wert: %f\n", result);
}

void testCrossProductV3(void) {
    vec3 a = {1.0f, 0.0f, 0.0f};
    vec3 b = {0.0f, 1.0f, 0.0f};
    vec3 result;
    crossProductV3(&result, &a, &b);

    vec3 expected = {0.0f, 0.0f, 1.0f}; // Erwartetes Kreuzprodukt

    // Überprüfen, ob das Ergebnis dem erwarteten Vektor entspricht
    bool isCorrect = fabs(result.x - expected.x) < 1e-5 &&
                     fabs(result.y - expected.y) < 1e-5 &&
                     fabs(result.z - expected.z) < 1e-5;

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: crossProductV3 hat das Kreuzprodukt der 3D-Vektoren korrekt berechnet.\n");
    } else {
        printf("Test fehlgeschlagen: crossProductV3 hat das Kreuzprodukt der 3D-Vektoren nicht korrekt berechnet.\n");
    }

    // Ausgabe des Ergebnis-Vektors zur Verifizierung
    printf("Ergebnis-Vektor:\n");
    printf("%f %f %f\n", result.x, result.y, result.z);
}


// transformations.c funktionen

void testLookAt(void) {
    mat4 result;
    vec3 eye = {0.0f, 0.0f, 5.0f}; // Augenposition
    vec3 target = {0.0f, 0.0f, 0.0f}; // Zielpunkt, auf den geschaut wird
    vec3 up = {0.0f, 1.0f, 0.0f}; // Up-Vektor

    // Aufruf der Funktion lookAt
    lookAt(&result, &eye, &target, &up);

    // Erwartete View-Matrix (lookAt Matrix)
    mat4 expected = {{
        1.000000f, 0.000000f, 0.000000f, 0.000000f,
        0.000000f, 1.000000f, 0.000000f, 0.000000f,
        0.000000f, 0.000000f, 1.000000f, 0.000000f,
        0.000000f, 0.000000f, -5.000000f, 1.000000f
    }};

    // Überprüfen, ob das Ergebnis der erwarteten Matrix entspricht
    bool isCorrect = true;
    for (int i = 0; i < 16; i++) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            isCorrect = false;
            break;
        }
    }

    // Konsolenausgabe
    if (isCorrect) {
        printf("Test erfolgreich: lookAt hat die korrekte View-Matrix erstellt.\n");
    } else {
        printf("Test fehlgeschlagen: lookAt hat die falsche View-Matrix erstellt.\n");
    }

    // Ausgabe der Ergebnis-Matrix zur Verifizierung
    printf("Ergebnis-Matrix:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", result.values[i * 4 + j]);
        }
        printf("\n");
    }
}


void testPerspective(void) {
    mat4 result;

    // Parameter für perspective-Funktion
    GLfloat yfov = 90.0f;  // Field of View in Grad
    GLfloat aspectRatio = 1.0f;  // Seitenverhältnis (width / height)
    GLfloat near = 1.0f;  // Near plane
    GLfloat far = 10.0f;  // Far plane

    // Erwartete Ergebnismatrix (vereinfachte Werte)
    mat4 expected = {
        1.0f, 0.0f,  0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,  0.0f,
        0.0f, 0.0f, -1.2222222f, -1.0f,
        0.0f, 0.0f, -2.2222222f,  0.0f
    };

    printf("Teste perspective:\n");

    // Funktion aufrufen
    perspective(&result, yfov, aspectRatio, near, far);

    // Vergleiche das Ergebnis mit dem Erwarteten
    int pass = 1;
    for (int i = 0; i < 16; ++i) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            pass = 0;
            break;
        }
    }

    // Gebe die resultierende Matrix aus
    printM4(&result);

    // Gebe das Testergebnis aus
    if (pass) {
        printf("Test bestanden!\n");
    } else {
        printf("Test fehlgeschlagen.\n");
    }
}

void testTranslate(void) {
    mat4 result;

    // Eingabematrix
    mat4 input = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        3.0f, 4.0f, 5.0f, 1.0f
    };

    // Translationsvektor
    vec3 translation = { 1.0f, 2.0f, 3.0f };

    // Erwartete Ergebnismatrix
    mat4 expected = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        4.0f, 6.0f, 8.0f, 1.0f
    };

    printf("Teste translate:\n");

    // Funktion aufrufen
    translate(&result, &input, &translation);

    // Vergleiche das Ergebnis mit dem Erwarteten
    int pass = 1;
    for (int i = 0; i < 16; ++i) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            pass = 0;
            break;
        }
    }

    // Gebe die resultierende Matrix aus
    printM4(&result);

    // Gebe das Testergebnis aus
    if (pass) {
        printf("Test bestanden!\n");
    } else {
        printf("Test fehlgeschlagen.\n");
    }
}

void testScale(void) {
    mat4 result;

    // Eingabematrix
    mat4 input = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Skalierungsvektor
    vec3 scaling = { 2.0f, 3.0f, 1.5f };

    // Erwartete Ergebnismatrix
    mat4 expected = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 3.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.5f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    printf("Teste scale:\n");

    // Funktion aufrufen
    scale(&result, &input, &scaling);

    // Vergleiche das Ergebnis mit dem Erwarteten
    int pass = 1;
    for (int i = 0; i < 16; ++i) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            pass = 0;
            break;
        }
    }

    // Gebe die resultierende Matrix aus
    printM4(&result);

    // Gebe das Testergebnis aus
    if (pass) {
        printf("Test bestanden!\n");
    } else {
        printf("Test fehlgeschlagen.\n");
    }
}

void testRotateX(void) {
    mat4 result;

    // Eingabematrix
    mat4 input = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Winkel in Grad
    GLfloat angle = 45.0f;

    // Erwartete Ergebnismatrix
    mat4 expected = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosf(angle*2.0f*M_PI/360.0f), sinf(angle*2.0f*M_PI/360.0f), 0.0f,
        0.0f, -sinf(angle*2.0f*M_PI/360.0f), cosf(angle*2.0f*M_PI/360.0f), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    printf("Teste rotatex:\n");

    // Funktion aufrufen
    rotatex(&result, &input, angle);

    // Vergleiche das Ergebnis mit dem Erwarteten
    int pass = 1;
    for (int i = 0; i < 16; ++i) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            pass = 0;
            break;
        }
    }

    // Gebe die resultierende Matrix aus
    printM4(&result);

    // Gebe das Testergebnis aus
    if (pass) {
        printf("Test bestanden!\n");
    } else {
        printf("Test fehlgeschlagen.\n");
    }
}

void testRotateY(void) {
    mat4 result;

    // Eingabematrix
    mat4 input = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Winkel in Grad
    GLfloat angle = 45.0f;

    // Erwartete Ergebnismatrix
    mat4 expected = {
        cosf(angle*2.0f*M_PI/360.0f), 0.0f, -sinf(angle*2.0f*M_PI/360.0f), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(angle*2.0f*M_PI/360.0f), 0.0f, cosf(angle*2.0f*M_PI/360.0f), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    printf("Teste rotatey:\n");

    // Funktion aufrufen
    rotatey(&result, &input, angle);

    // Vergleiche das Ergebnis mit dem Erwarteten
    int pass = 1;
    for (int i = 0; i < 16; ++i) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            pass = 0;
            break;
        }
    }

    // Gebe die resultierende Matrix aus
    printM4(&result);

    // Gebe das Testergebnis aus
    if (pass) {
        printf("Test bestanden!\n");
    } else {
        printf("Test fehlgeschlagen.\n");
    }
}

void testRotateZ(void) {
    mat4 result;

    // Eingabematrix
    mat4 input = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Winkel in Grad
    GLfloat angle = 90.0f;

    // Erwartete Ergebnismatrix
    mat4 expected = {
        cosf(angle*2.0f*M_PI/360.0f), sinf(angle*2.0f*M_PI/360.0f), 0.0f, 0.0f,
        -sinf(angle*2.0f*M_PI/360.0f), cosf(angle*2.0f*M_PI/360.0f), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    printf("Teste rotatez:\n");

    // Funktion aufrufen
    rotatez(&result, &input, angle);

    // Vergleiche das Ergebnis mit dem Erwarteten
    int pass = 1;
    for (int i = 0; i < 16; ++i) {
        if (fabs(result.values[i] - expected.values[i]) > 1e-5) {
            pass = 0;
            break;
        }
    }

    // Gebe die resultierende Matrix aus
    printM4(&result);

    // Gebe das Testergebnis aus
    if (pass) {
        printf("Test bestanden!\n");
    } else {
        printf("Test fehlgeschlagen.\n");
    }
}

int main(void) {
    // mat.c funktionen

    testNullM4();
    printf("\n");
    testNullM3();
    printf("\n");
    testIdentityM4();
    printf("\n");
    testIdentityM3();
    printf("\n");
    testMultiplyM4xM4();
    printf("\n");
    testMultiplyM3xM3();
    printf("\n");
    testMultiplyM4xV4();
    printf("\n");
    testMultiplyM3xV3();
    printf("\n");
    testDet2x2();
    printf("\n");
    testInverteAndTransposeM3();
    printf("\n");
    testInverteM3();
    printf("\n");
    testTransposeM4();
    printf("\n");
    testTransposeM3();
    printf("\n");
    testMat4x4ToMat3x3();
    printf("\n");
    testPrintM4();
    printf("\n");
    testPrintM3();
    printf("\n");

    // vec.c funktionen

    testVec3ToVec4();
    printf("\n");
    testVec4ToVec3();
    printf("\n");
    testVec4ToHomogeneousVec3();
    printf("\n");
    testGetLenghtV3();
    printf("\n");
    testGetLenghtV4();
    printf("\n");
    testNormalizeV3();
    printf("\n");
    testNormalizeV4();
    printf("\n");
    testDotProductV3();
    printf("\n");
    testDotProductV4();
    printf("\n");
    testCrossProductV3();
    printf("\n");

    //transformations.c funktionen

    testLookAt();
    printf("\n");
    testPerspective();
    printf("\n");
    testTranslate();
    printf("\n");
    testScale();
    printf("\n");
    testRotateX();
    printf("\n");
    testRotateY();
    printf("\n");
    testRotateZ();
    printf("\n");

    return 0;
}
