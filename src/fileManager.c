#ifdef __CYGWIN__
// Wenn das Programm unter Cygwin kompiliert wird, wird die Windows-Headerdatei eingebunden
#include <windows.h>
#endif

#include <stdio.h> // Für Dateiein- und -ausgabe und Standard I/O Funktionen
#include <stdlib.h> // Für Speicherverwaltung und Standardbibliotheksfunktionen

#include "objStructs.h" // Benutzerdefinierte Header-Datei für Datenstrukturen

// Funktion, um den Inhalt einer Datei zu lesen und als Zeichenkette zurückzugeben
const char* getFileContent(const char* const fileName)
{
    FILE *file = fopen(fileName, "rb"); // Datei im Binärmodus öffnen

    if (file == NULL)
    {
        printf("Could not open file : %s\n", fileName); // Fehlermeldung, falls Datei nicht geöffnet werden kann
        return NULL;
    }

    char *buffer;
    int length;

    if (fseek(file, 0, SEEK_END) < 0) // Datei-Zeiger ans Ende der Datei setzen
    {
        printf("fseek threw an error\n");
        return NULL;
    }

    length = ftell(file); // Aktuelle Position des Datei-Zeigers (Dateigröße)

    if (length < 0)
    {
        printf("ftell threw an error\n");
        return NULL;
    }

    if (fseek(file, 0, SEEK_SET) < 0) // Datei-Zeiger zurück an den Anfang der Datei setzen
    {
        printf("fseek threw an error\n");
        return NULL;
    }

    buffer = (char *)malloc(length * sizeof(char) + 1); // Speicher für den Dateiinhalte allozieren

    if (buffer == NULL)
    {
        printf("malloc could not allocate memory\n");
        return NULL;
    }

    fread(buffer, sizeof(char), length, file); // Dateiinhalt in den Puffer lesen

    buffer[length] = '\0'; // Null-terminierte Zeichenkette

    if (fclose(file) == EOF) // Datei schließen
    {
        printf("Error while closing file : %s\n", fileName);
        return NULL;
    }

    return buffer; // Zeichenkette zurückgeben
}

// Funktion, um OBJ-Dateien zu laden und die enthaltenen Vertizes auszulesen
vertex* loadObjFromFile(const char* const fileName, int* const numOfVertecies)
{
    FILE *file = fopen(fileName, "rb"); // Datei im Binärmodus öffnen

    if (file == NULL)
    {
        printf("Could not open file : %s\n", fileName); // Fehlermeldung, falls Datei nicht geöffnet werden kann
        return NULL;
    }

    int vertexBufferSize = 0;
    int vertexPositionBufferSize = 0;
    int vertexTextureBufferSize = 0;
    int vertexNormalBufferSize = 0;

    char* lineData = NULL;
    size_t lineDataSize = 0;
    int bytesRead;

    // Erste Schleife zum Zählen der verschiedenen Vertex-Typen
    while (1)
    {
        bytesRead = getline(&lineData, &lineDataSize, file);

        if (bytesRead < 0)
        {
            break;
        }

        if (bytesRead < 3)
        {
            continue;
        }

        // Zählen der Positions-, Normalen- und Texturkoordinaten-Vertizes
        if (lineData[0] == 'v' && lineData[1] == ' ')
        {
            vertexPositionBufferSize++;
        }
        else if (lineData[0] == 'v' && lineData[1] == 'n' && lineData[2] == ' ')
        {
            vertexNormalBufferSize++;
        }
        else if (lineData[0] == 'v' && lineData[1] == 't' && lineData[2] == ' ')
        {
            vertexTextureBufferSize++;
        }
        else if (lineData[0] == 'f' && lineData[1] == ' ')
        {
            vertexBufferSize += 3; // Dreieck aus 3 Vertizes
        }
    }

    // Datei-Zeiger wieder an den Anfang setzen
    if (fseek(file, 0, SEEK_SET) < 0)
    {
        printf("fseek threw an error\n");
        return NULL;
    }

    // Überprüfen, ob die Datei gültige Vertex-Daten enthält
    if (vertexBufferSize == 0)
    {
        printf("Obj file does not define any vertecies\n");
        return NULL;
    }

    if (vertexPositionBufferSize == 0)
    {
        printf("Obj file does not define vertex positions\n");
        return NULL;
    }

    if (vertexTextureBufferSize == 0)
    {
        printf("Obj file does not define texture coordinates\n");
        return NULL;
    }

    if (vertexNormalBufferSize == 0)
    {
        printf("Obj file does not define vertex normals\n");
        return NULL;
    }

    // Speicher für die Vertizes und deren Attribute allozieren
    vertex *vertecies = (vertex *)malloc(vertexBufferSize * sizeof(vertex)); 
    vec3 *vertexPositions = (vec3 *)malloc(vertexPositionBufferSize * sizeof(vec3));
    vec2 *textureCoordinates = (vec2 *)malloc(vertexTextureBufferSize * sizeof(vec2));
    vec3 *vertexNormals = (vec3 *)malloc(vertexNormalBufferSize * sizeof(vec3));

    *numOfVertecies = 0; // Zähler für die Anzahl der Vertizes
    int vertexPositionsSaved = 0;
    int vertexTextureCoordSaved = 0;
    int vertexNormalsSaved = 0;

    // Zweite Schleife zum Einlesen und Speichern der Vertex-Daten
    while (1)
    {
        bytesRead = getline(&lineData, &lineDataSize, file);

        if (bytesRead < 0)
        {
            break;
        }

        if (bytesRead < 3)
        {
            continue;
        }

        // Einlesen der Vertex-Positionen
        if (lineData[0] == 'v' && lineData[1] == ' ')
        {
            sscanf(lineData, "%*s %f %f %f", &vertexPositions[vertexPositionsSaved].x, &vertexPositions[vertexPositionsSaved].y, &vertexPositions[vertexPositionsSaved].z);
            vertexPositionsSaved++;
        }
        // Einlesen der Vertex-Normalen
        else if (lineData[0] == 'v' && lineData[1] == 'n' && lineData[2] == ' ')
        {
            sscanf(lineData, "%*s %f %f %f", &vertexNormals[vertexNormalsSaved].x, &vertexNormals[vertexNormalsSaved].y, &vertexNormals[vertexNormalsSaved].z);
            vertexNormalsSaved++;
        }
        // Einlesen der Texturkoordinaten
        else if (lineData[0] == 'v' && lineData[1] == 't' && lineData[2] == ' ')
        {
            sscanf(lineData, "%*s %f %f", &textureCoordinates[vertexTextureCoordSaved].x, &textureCoordinates[vertexTextureCoordSaved].y);
            vertexTextureCoordSaved++;
        }
        // Einlesen der Face-Daten und Zuordnung zu den Vertizes
        else if (lineData[0] == 'f' && lineData[1] == ' ')
        {
            int posIndex[3], textureCoord[3], normalIndex[3];
            sscanf(lineData, "%*s %d/%d/%d %d/%d/%d %d/%d/%d", posIndex, textureCoord, normalIndex, posIndex + 1, textureCoord + 1, normalIndex + 1, posIndex + 2, textureCoord + 2, normalIndex + 2);

            for (int i = 0; i < 3; i++)
            {
                vertecies[*numOfVertecies + i].position = vertexPositions[posIndex[i] - 1];
                vertecies[*numOfVertecies + i].textureCoord = textureCoordinates[textureCoord[i] - 1];
                vertecies[*numOfVertecies + i].normal = vertexNormals[normalIndex[i] - 1];
            }

            *numOfVertecies += 3; // Zähler für Vertizes erhöhen
        }
    }

    // Speicher freigeben
    free(lineData);
    lineData = NULL;
    free(vertexPositions);
    vertexPositions = NULL;
    free(textureCoordinates);
    textureCoordinates = NULL;
    free(vertexNormals);
    vertexNormals = NULL;
    
    if (fclose(file) == EOF) // Datei schließen
    {
        printf("Error while closing file : %s\n", fileName);
        return NULL;
    }

    file = NULL;

    return vertecies; // Vertizes zurückgeben
}
