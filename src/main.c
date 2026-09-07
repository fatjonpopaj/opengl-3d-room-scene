#include <stdio.h> // Standard I/O Funktionen
#include <stdlib.h> // Standardbibliotheksfunktionen
#include <math.h> // Mathematische Funktionen

#ifdef __CYGWIN__
// Wenn das Programm unter Cygwin kompiliert wird, wird die Windows-Headerdatei eingebunden
#include <windows.h>
#endif

#ifndef M_PI
// Definition der Konstanten für Pi, falls nicht bereits definiert
#define M_PI 3.14159265358979323846
#endif

#include <GL/glew.h> // OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h> // OpenGL Framework Library

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h" // STB Bildbibliothek für das Laden von Bildern

#include "objStructs.h" // Benutzerdefinierte Header-Datei für Datenstrukturen
#include "fileManager.h"
#include "transformations.h"
#include "mat.h"
#include "vec.h"
#include "renderer.h"

// Anfangsgröße des Fensters
#define START_WINDOW_WIDTH 900
#define START_WINDOW_HEIGHT 600

// Shader-Dateien
#define VERTEX_SHADER_FILENAME "shader/vertexShader.glsl"
#define FRAGMENT_SHADER_FILENAME "shader/fragmentShader.glsl"
#define VERTEX2_SHADER_FILENAME "shader/vertexShader2.glsl"
#define FRAGMENT2_SHADER_FILENAME "shader/fragmentShader2.glsl"

GLfloat lightOffset = 0.0;

// Globale Variablen für Fensterbreite, -höhe und Seitenverhältnis
GLint width, height;
GLfloat aspectRatio;

// Globale Variablen für Shader-Programme
GLuint program;
GLuint program2;

// Definition eines Lichtquellen-Objekts
lightObject lightSourceObj1 =
{
    .position = {-4.4f, 6.0f, 0.0f}, // Position der Lichtquelle
    .lightMaterial = 
    {
        .ambientColor = {0.3f, 0.22f, 0.22f}, // Umgebungsfarbe des Lichts
        .diffuseColor = {0.8f, 0.8f, 0.8f}, // Diffuse Farbe des Lichts
        .specularColor = {0.9f, 0.9f, 0.9f} // Spiegelnde Farbe des Lichts
    }
};

// Definition einer zweiten Lichtquelle
lightObject lightSourceObj2 =
{
    .position = {4.4f, 6.0f, 0.0f}, // Position der Lichtquelle
    .lightMaterial = 
    {
        .ambientColor = {0.22f, 0.22f, 0.3f}, // Umgebungsfarbe des Lichts
        .diffuseColor = {0.8f, 0.8f, 0.8f}, // Diffuse Farbe des Lichts
        .specularColor = {0.9f, 0.9f, 0.9f} // Spiegelnde Farbe des Lichts
    }
};

// Definition der Hauptkamera
cameraObj camera =
{
    .transform =
    {
        .position = {0.0f, 5.0f, 4.0f}, // Position der Kamera
        .rotation = {0,0,-1}, // Rotation der Kamera
        .scale = {1,1,1} // Skalierung der Kamera
    },
    .up = {0,1,0}, // "Oben"-Richtung der Kamera
    .fov = 110, // Sichtfeld der Kamera
    .nearPlane = 0.1f, // Nahe Sichtgrenze der Kamera
    .farPlane = 16.0f, // Ferne Sichtgrenze der Kamera
    .aspectRatio = 1.0f // Seitenverhältnis der Kamera
};

// Definition einer Spiegelkamera
cameraObj mirrorCamera = 
{
    .transform =
    {
        .position = {0.0f, 4.0f, 6.0f}, // Position der Spiegelkamera
        .rotation = {0, 0, -1}, // Rotation der Spiegelkamera
        .scale = {1,1,1} // Skalierung der Spiegelkamera
    },
    .up = {0,1,0}, // "Oben"-Richtung der Kamera
    .nearPlane = 0.1f, // Nahe Sichtgrenze der Kamera
    .farPlane = 64.0f, // Ferne Sichtgrenze der Kamera
    .aspectRatio = 1, // Seitenverhältnis der Kamera
    .fov = 1 // Sichtfeld der Kamera
};

// Funktion zum Erstellen eines Shaders
GLuint createShader(GLenum type, const char* const shaderFileName)
{
    // Shader-Quellcode aus Datei lesen
    const GLchar* shaderSource = getFileContent(shaderFileName);

    // Erstellen eines Shaders des angegebenen Typs (Vertex, Fragment, etc.)
    GLuint shader = glCreateShader(type);

    // Setzen des Shader-Quellcodes
    glShaderSource(shader, 1, &shaderSource, NULL);

    // Freigeben des Speicherbereichs, der den Quellcode enthält
    free((void*) shaderSource);
    shaderSource = NULL;

    // Kompilieren des Shaders
    glCompileShader(shader);

    // Überprüfen des Kompilierungsstatus
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    // Wenn ein Fehler auftritt, Fehlermeldung ausgeben und Fehlerprotokoll anzeigen
    if (!status)
    {
        printf("Error while compiling shader from file : %s\n", shaderFileName);
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
        return -1;
    }

    // Rückgabe des kompilierten Shaders
    return shader;
}

// Funktion zum Erstellen eines Shader-Programms aus einer Liste von Shaders
GLuint createProgram(GLuint* shaderList, int shaderCount)
{
    // Erstellen eines neuen Shader-Programms
    GLuint newProgram = glCreateProgram();

    // Anhängen jedes Shaders an das Programm
    for (int i = 0; i < shaderCount; i++)
    {
        glAttachShader(newProgram, shaderList[i]);
    }

    // Verlinken des Programms
    glLinkProgram(newProgram);

    // Überprüfen des Link-Status
    GLint status;
    glGetProgramiv(newProgram, GL_LINK_STATUS, &status);

    // Wenn ein Fehler auftritt, Fehlermeldung ausgeben und Fehlerprotokoll anzeigen
    if (!status)
    {
        printf("Error while linking shader program\n");
        char infoLog[1024];
        glGetProgramInfoLog(newProgram, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
        return -1;
    }

    // Rückgabe des verlinkten Programms
    return newProgram;
}

// Funktion zum Erstellen einer Textur aus einer PNG-Datei
GLuint createTextureFromPNG(const char* const shaderFileName)
{
    int width, height, nrChannels;
    // Bilddaten aus der Datei laden
    unsigned char *image = stbi_load(shaderFileName, &width, &height, &nrChannels, 0);

    // Fehlerprüfung beim Laden des Bildes
    if(image == NULL) 
    {
        printf("Error in loading the image\n");
        return -1;
    }

    // Erstellen eines neuen Textur-Objekts
    GLuint newTexture;
    glGenTextures(1, &newTexture);
    glBindTexture(GL_TEXTURE_2D, newTexture);

    // Setzen der Texturparameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bilddaten in die Textur laden
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); 
    glGenerateMipmap(GL_TEXTURE_2D);

    // Textur unbinden und Bilddaten freigeben
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    // Rückgabe des Textur-Objekts
    return newTexture;
}

// Funktion zum Erstellen einer Textur aus einer JPEG-Datei
GLuint createTextureFromJPEG(const char* const shaderFileName)
{
    int width, height, nrChannels;
    // Bilddaten aus der Datei laden
    unsigned char *image = stbi_load(shaderFileName, &width, &height, &nrChannels, 0);

    // Fehlerprüfung beim Laden des Bildes
    if(image == NULL) 
    {
        printf("Error in loading the image\n");
        return -1;
    }

    // Erstellen eines neuen Textur-Objekts
    GLuint newTexture;
    glGenTextures(1, &newTexture);
    glBindTexture(GL_TEXTURE_2D, newTexture);

    // Setzen der Texturparameter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bilddaten in die Textur laden
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); 
    glGenerateMipmap(GL_TEXTURE_2D);

    // Textur unbinden und Bilddaten freigeben
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    // Rückgabe des Textur-Objekts
    return newTexture;
}

// Funktion zum Erstellen eines Game-Objekts aus einer Datei und einem Material
gameObjectData createGameObject(const char* const fileName, objMaterial* material)
{
    // Laden der Vertices aus der Datei
    GLint vertexCount;
    vertex* vertecies = loadObjFromFile(fileName, &vertexCount);

    // Allokieren des Speichers für die Indizes
    GLuint* indicies = (GLuint *)malloc(vertexCount * sizeof(GLuint)); 

    // Setzen der Indizes
    for (int i = 0; i < vertexCount; i++)
    {
        indicies[i] = i;
    }

    // Erstellen eines gameObjectData und Rückgabe
    return (gameObjectData)
    {
        .vertecies = vertecies,
        .indicies = indicies,
        .vertexCount = vertexCount,
        .indexCount = vertexCount,
        .material = *material
    };
}

// Funktion zum Binden eines Game-Objekts an ein VAO
GLuint bindGameObjectToVAO(gameObjectData* gameObject)
{
    // Fehlerprüfung auf Nullzeiger
    if (gameObject == NULL || gameObject->vertecies == NULL || gameObject->indicies == NULL)
    {
        printf("Error loading obj data\n");
        return -1;
    }

    // Erstellen und Binden eines Vertex Buffer Objects (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, gameObject->vertexCount * sizeof(vertex), gameObject->vertecies, GL_STATIC_DRAW);
    free(gameObject->vertecies); // Freigeben des Speichers für die Vertices
    gameObject->vertecies = NULL;
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Erstellen und Binden eines Index Buffer Objects (IBO)
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gameObject->indexCount * sizeof(GLuint), gameObject->indicies, GL_STATIC_DRAW);
    free(gameObject->indicies); // Freigeben des Speichers für die Indizes
    gameObject->indicies = NULL;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Erstellen und Binden eines Vertex Array Objects (VAO)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Setzen der Vertex-Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(sizeof(vec3))); // Normale
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(2*sizeof(vec3))); // Texturkoordinaten
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Rückgabe des VAO
    return vao;
}

// Funktion zur Berechnung der Kamera für den Spiegel
void calculateMirrorCamera(renderObject* mirror) // Diese Funktion geht davon aus, dass die Normale auf der positiven oder negativen Z-Achse liegt
{
    // Setzen der Normalen des Spiegels
    vec3 mirrorNormal = {0, 0, -1}; 
    
    // Berechnung der Position der invertierten Kamera
    vec3 invertedPos = 
    {
        .x = (camera.transform.position.x - mirror->transform.position.x) * mirrorNormal.x,
        .y = (camera.transform.position.y - mirror->transform.position.y) * mirrorNormal.y,
        .z = (camera.transform.position.z - mirror->transform.position.z) * mirrorNormal.z
    };

    // Spiegeln der Position der Kamera
    invertedPos = (vec3)
    {
        .x = camera.transform.position.x + invertedPos.x * 2,
        .y = camera.transform.position.y + invertedPos.y * 2,
        .z = camera.transform.position.z + invertedPos.z * 2
    };

    // Setzen der Position der Spiegelkamera
    mirrorCamera.transform.position = invertedPos;

    // Berechnung der Blickrichtung der Spiegelkamera
    vec3 lookAt =
    {
        .x = mirror->transform.position.x - invertedPos.x,
        .y = mirror->transform.position.y - invertedPos.y,
        .z = mirror->transform.position.z - invertedPos.z
    };

    // Setzen der Rotation der Spiegelkamera
    mirrorCamera.transform.rotation = lookAt;

    /*  a^2 = b^2 + c^2 -2*b*c*cos(A)  Kosinussatz

       A
       |\   
     b | \ c
       ---
        a
    */
    
    // Berechnung des Sichtfelds der Kamera basierend auf der Höhe des Spiegels
    GLfloat a, b, c;
    a = mirror->transform.scale.y;
    
    // Berechnung des Abstands von der Kamera zum oberen Punkt des Spiegels
    vec3 cameraToUpperPoint = 
    {
        .x = (mirror->transform.position.x - mirrorCamera.transform.position.x),
        .y = ((mirror->transform.position.y + (a / 2.0f)) - mirrorCamera.transform.position.y),
        .z = (mirror->transform.position.z - mirrorCamera.transform.position.z)
    };
    
    b = sqrtf(cameraToUpperPoint.x * cameraToUpperPoint.x + cameraToUpperPoint.y * cameraToUpperPoint.y + cameraToUpperPoint.z * cameraToUpperPoint.z);
    
    // Berechnung des Abstands von der Kamera zum unteren Punkt des Spiegels
    vec3 cameraToLowerPoint = 
    {
        .x = ((mirror->transform.position.x) - mirrorCamera.transform.position.x),
        .y = ((mirror->transform.position.y - (a / 2.0f)) - mirrorCamera.transform.position.y),
        .z = ((mirror->transform.position.z) - mirrorCamera.transform.position.z)
    };

    c = sqrtf(cameraToLowerPoint.x * cameraToLowerPoint.x + cameraToLowerPoint.y * cameraToLowerPoint.y + cameraToLowerPoint.z * cameraToLowerPoint.z);

    // Berechnung des Öffnungswinkels der Kamera basierend auf dem Kosinussatz
    GLfloat angle = (acosf((b * b + c * c - a * a) / (2.0f * b * c)) * 360.0f) / (2.0f * M_PI);

    // Setzen des Sichtfelds der Spiegelkamera
    mirrorCamera.fov = angle;

    // Berechnung des Seitenverhältnisses der Spiegelkamera
    vec3 pos1 =
    {
        .x = mirrorCamera.transform.position.x - mirror->transform.position.x,
        .y = 0,
        .z = mirrorCamera.transform.position.z - mirror->transform.position.z
    };

    vec3 pos2 =
    {
        .x = camera.transform.position.x - mirror->transform.position.x,
        .y = 0,
        .z = camera.transform.position.z - mirror->transform.position.z
    };

    // Normalisieren des Vektors
    normalizeV3(&pos1);
    normalizeV3(&pos2);

    // Setzen des Seitenverhältnisses der Spiegelkamera
    mirrorCamera.aspectRatio = (-dotProductV3(&pos1, &pos2)+1)/2.0f;
}

mat4 viewProjectionMatMatL;

renderObject objects[32]; // Array von Render-Objekten
GLint objectCount; // Anzahl der Objekte

// Initialisierungsfunktion
void init(void)
{
    // Shaders erstellen
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, VERTEX_SHADER_FILENAME);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_FILENAME);
    GLuint vertexShader2 = createShader(GL_VERTEX_SHADER, VERTEX2_SHADER_FILENAME);
    GLuint fragmentShader2 = createShader(GL_FRAGMENT_SHADER, FRAGMENT2_SHADER_FILENAME);

    // Shader-Programme erstellen
    program = createProgram((GLuint[]) {vertexShader, fragmentShader}, 2);
    program2 = createProgram((GLuint[]) {vertexShader2, fragmentShader2}, 2);

    // OpenGL-Einstellungen
    glEnable(GL_CULL_FACE); // Rückseitenverdeckung aktivieren
    glEnable(GL_DEPTH_TEST); // Tiefentest aktivieren

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // Hintergrundfarbe setzen
    glViewport(0, 0, START_WINDOW_WIDTH, START_WINDOW_HEIGHT); // Viewport setzen
    aspectRatio = START_WINDOW_WIDTH / (GLfloat)START_WINDOW_HEIGHT;
    camera.aspectRatio = aspectRatio;

    // Texturen erstellen
    GLuint whiteTexture = createTextureFromJPEG("tex/white.jpg");
    GLuint tableTexture = createTextureFromJPEG("tex/wood.jpg");
    GLuint ventilatorBlaetterTexture = createTextureFromJPEG("tex/ventilatorBlaetter.jpg");
    GLuint ventilatorBasisTexture = createTextureFromJPEG("tex/ventilatorBasis.jpg");
    GLuint brettTexture = createTextureFromPNG("tex/brett.png");
    GLuint weisseFigurenTexture = createTextureFromJPEG("tex/weisseFiguren.jpg");
    GLuint schwarzeFigurenTexture = createTextureFromJPEG("tex/schwarzeFiguren.jpg");
    GLuint bodenTexture = createTextureFromJPEG("tex/boden.jpg");

    // Materialien definieren
    objMaterial woodMaterial = (objMaterial)
    {
        .emissiveColor = {0.0f, 0.0f, 0.0f},
        .ambientColor = {0.6f, 0.6f, 0.6f},
        .diffuseColor = {0.7f, 0.7f, 0.7f},
        .specularColor = {0.2f, 0.2f, 0.2f},
        .shininess = 30.0f,
        .texture = tableTexture
    };

    objMaterial weisseFigurenMaterial = (objMaterial)
    {
        .emissiveColor = {0.0f, 0.0f, 0.0f},
        .ambientColor = {0.5f, 0.5f, 0.5f},
        .diffuseColor = {0.7f, 0.7f, 0.7f},
        .specularColor = {0.2f, 0.2f, 0.2f},
        .shininess = 30.0f,
        .texture = weisseFigurenTexture
    };

    objMaterial schwarzeFigurenMaterial = (objMaterial)
    {
        .emissiveColor = {0.0f, 0.0f, 0.0f},
        .ambientColor = {0.6f, 0.6f, 0.6f},
        .diffuseColor = {0.7f, 0.7f, 0.7f},
        .specularColor = {0.2f, 0.2f, 0.2f},
        .shininess = 30.0f,
        .texture = schwarzeFigurenTexture
    };

    objMaterial brettMaterial = (objMaterial)
    {
        .emissiveColor = {0.0f, 0.0f, 0.0f},
        .ambientColor = {0.6f, 0.6f, 0.6f},
        .diffuseColor = {0.6f, 0.6f, 0.6f},
        .specularColor = {0.5f, 0.5f, 0.5f},
        .shininess = 30.0f,
        .texture = brettTexture
    };

    objMaterial wandMaterial = (objMaterial)
    {
        .emissiveColor = {0.05f, 0.05f, 0.05f},
        .ambientColor = {0.2f, 0.2f, 0.2f},
        .diffuseColor = {0.2f, 0.2f, 0.2f},
        .specularColor = {0.05f, 0.05f, 0.05f},
        .shininess = 10.0f,
        .texture = whiteTexture
    };

    objMaterial bodenMaterial = (objMaterial)
    {
        .emissiveColor = {0.05f, 0.05f, 0.05f},
        .ambientColor = {0.2f, 0.2f, 0.2f},
        .diffuseColor = {0.2f, 0.2f, 0.2f},
        .specularColor = {0.05f, 0.05f, 0.05f},
        .shininess = 10.0f,
        .texture = bodenTexture
    };

    objMaterial ventilatorBlaetterMaterial = (objMaterial)
    {
        .emissiveColor = {0.05f, 0.05f, 0.05f},
        .ambientColor = {0.25f, 0.25f, 0.25f},
        .diffuseColor = {0.4f, 0.4f, 0.4f},
        .specularColor = {0.75f, 0.75f, 0.75f},
        .shininess = 75.0f,
        .texture = ventilatorBlaetterTexture
    };

    objMaterial ventilatorBasisMaterial = (objMaterial)
    {
        .emissiveColor = {0.05f, 0.05f, 0.05f},
        .ambientColor = {0.25f, 0.3f, 0.25f},
        .diffuseColor = {0.45f, 0.4f, 0.4f},
        .specularColor = {0.75f, 0.75f, 0.75f},
        .shininess = 75.0f,
        .texture = ventilatorBasisTexture
    };

    objMaterial ventilatorStangeMaterial = (objMaterial)
    {
        .emissiveColor = {0.05f, 0.05f, 0.05f},
        .ambientColor = {0.25f, 0.25f, 0.25f},
        .diffuseColor = {0.4f, 0.4f, 0.4f},
        .specularColor = {0.8f, 0.8f, 0.8f},
        .shininess = 150.0f,
        .texture = whiteTexture
    };

    objMaterial lampeInnenMaterial = (objMaterial)
    {
        .emissiveColor = {0.2f, 0.2f, 0.2f},
        .ambientColor = {0.3f, 0.3f, 0.3f},
        .diffuseColor = {1.0f, 1.0f, 1.0f},
        .specularColor = {0.2f, 0.2f, 0.2f},
        .shininess = 100.0f,
        .texture = whiteTexture
    };

    objMaterial mirrorMaterial = (objMaterial)
    {
        .emissiveColor = {0.6f, 0.6f, 0.6f},
        .ambientColor = {0.1f, 0.1f, 0.1f},
        .diffuseColor = {0.0f, 0.0f, 0.0f},
        .specularColor = {0.0f, 0.0f, 0.0f},
        .shininess = 1.0f,
        .texture = whiteTexture
    };

    // Game-Objekte erstellen und an VAOs binden
    gameObjectData stuhlData = createGameObject("obj/stuhl.obj", &woodMaterial);
    GLuint stuhlVAO = bindGameObjectToVAO(&stuhlData);

    gameObjectData tischData = createGameObject("obj/tisch.obj", &woodMaterial);
    GLuint tischVAO = bindGameObjectToVAO(&tischData);

    gameObjectData wandData = createGameObject("obj/boden.obj", &wandMaterial);
    GLuint wandVAO = bindGameObjectToVAO(&wandData);

    gameObjectData bodenData = createGameObject("obj/boden.obj", &bodenMaterial);
    GLuint bodenVAO = bindGameObjectToVAO(&bodenData);

    gameObjectData mirrorData = createGameObject("obj/wand.obj", &mirrorMaterial);
    GLuint mirrorVAO = bindGameObjectToVAO(&mirrorData);

    gameObjectData ventilatorBasisData = createGameObject("obj/ventilatorBasis.obj", &ventilatorBasisMaterial);
    GLuint ventilatorBasisVAO = bindGameObjectToVAO(&ventilatorBasisData);

    gameObjectData ventilatorBlaetterData = createGameObject("obj/ventilatorBlaetter.obj", &ventilatorBlaetterMaterial);
    GLuint ventilatorBlaetterVAO = bindGameObjectToVAO(&ventilatorBlaetterData);

    gameObjectData ventilatorStangeData = createGameObject("obj/ventilatorStange.obj", &ventilatorStangeMaterial);
    GLuint ventilatorStangeVAO = bindGameObjectToVAO(&ventilatorStangeData);

    gameObjectData lampeInnenData = createGameObject("obj/lampeInnen.obj", &lampeInnenMaterial);
    GLuint lampeInnenVAO = bindGameObjectToVAO(&lampeInnenData);

    gameObjectData lampeAussenData = createGameObject("obj/lampeAussen.obj", &woodMaterial);
    GLuint lampeAussenVAO = bindGameObjectToVAO(&lampeAussenData);

    gameObjectData whiteData = createGameObject("obj/weisseFiguren.obj", &weisseFigurenMaterial);
    GLuint whiteVAO = bindGameObjectToVAO(&whiteData);

    gameObjectData blackData = createGameObject("obj/schwarzeFiguren.obj", &schwarzeFigurenMaterial);
    GLuint blackVAO = bindGameObjectToVAO(&blackData);

    gameObjectData brettAussenData = createGameObject("obj/brettAussen.obj", &brettMaterial);
    GLuint brettAussenVAO = bindGameObjectToVAO(&brettAussenData);

    gameObjectData brettObenData = createGameObject("obj/brettOben.obj", &brettMaterial);
    GLuint brettObenVAO = bindGameObjectToVAO(&brettObenData);

    // Objekte in Szene setzen

    // Stuhl Links
    gameObject transform0 =
    {
        .position = {-2.0f, 0.0f, 0.0f},
        .rotation = {0,90,0},
        .scale = {1,1,1}
    };
    objects[0] = (renderObject) {.transform = transform0, .data = stuhlData, .vao = stuhlVAO};

    // Stuhl Rechts
    gameObject transform1 =
    {
        .position = {2.0f, 0.0f, 0.0f},
        .rotation = {0,-90,0},
        .scale = {1,1,1}
    };
    objects[1] = (renderObject) {.transform = transform1, .data = stuhlData, .vao = stuhlVAO};

    // Tisch
    gameObject transform2 =
    {
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {0,-90,0},
        .scale = {1,1,1}
    };
    objects[2] = (renderObject) {.transform = transform2, .data = tischData, .vao = tischVAO};

    // Boden
    gameObject transform3 =
    {
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {-90,0,0},
        .scale = {10,10,10}
    };
    objects[3] = (renderObject) {.transform = transform3, .data = bodenData, .vao = bodenVAO};

    // Wand Links
    gameObject transform4 =
    {
        .position = {-5.0f, 5.0f, 0.0f},
        .rotation = {0,90,0},
        .scale = {10,10,10}
    };
    objects[4] = (renderObject) {.transform = transform4, .data = wandData, .vao = wandVAO};

    // Wand Hinten
    gameObject transform5 =
    {
        .position = {0.0f, 5.0f, -5.0f},
        .rotation = {0,0,0},
        .scale = {10,10,10}
    };
    objects[5] = (renderObject) {.transform = transform5, .data = wandData, .vao = wandVAO};

    // Wand Rechts
    gameObject transform6 =
    {
        .position = {5.0f, 5.0f, .0f},
        .rotation = {0,-90,0},
        .scale = {10,10,10}
    };
    objects[6] = (renderObject) {.transform = transform6, .data = wandData, .vao = wandVAO};

    // Wand Vorne
    gameObject transform7 =
    {
        .position = {0.0f, 5.0f, 5.0f},
        .rotation = {0,180,0},
        .scale = {10,10,10}
    };
    objects[7] = (renderObject) {.transform = transform7, .data = wandData, .vao = wandVAO};

    // Decke
    gameObject transform8 =
    {
        .position = {0.0f, 10.0f, 0.0f},
        .rotation = {90,0,0},
        .scale = {10,10,10}
    };
    objects[8] = (renderObject) {.transform = transform8, .data = wandData, .vao = wandVAO};

    // Spiegel
    gameObject transform9 =
    {
        .position = {0.0f, 5.0f, -4.99f},
        .rotation = {0,0,0},
        .scale = {6,6,1}
    };
    objects[9] = (renderObject) {.transform = transform9, .data = mirrorData, .vao = mirrorVAO};

    // Ventilator-Basis
    gameObject transform10 =
    {
        .position = {0.0f, 11.0f, 0.0f},
        .rotation = {0,0,0},
        .scale = {0.5f,1,0.5f}
    };
    objects[10] = (renderObject) {.transform = transform10, .data = ventilatorBasisData, .vao = ventilatorBasisVAO};

    // Ventilator-Blätter
    gameObject transform11 =
    {
        .position = {0.0f, 11.0f, 0.0f},
        .rotation = {0,0,0},
        .scale = {0.5f,1,0.5f}
    };
    objects[11] = (renderObject) {.transform = transform11, .data = ventilatorBlaetterData, .vao = ventilatorBlaetterVAO};

    // Ventilator-Stange
    gameObject transform12 =
    {
        .position = {0.0f, 11.0f, 0.0f},
        .rotation = {0,0,0},
        .scale = {0.7f,1,0.7f}
    };
    objects[12] = (renderObject) {.transform = transform12, .data = ventilatorStangeData, .vao = ventilatorStangeVAO};

    // Lampe Innen rechts
    gameObject transform13 =
    {
        .position = (vec3){lightSourceObj2.position.x + 0.62f, lightSourceObj2.position.y + 0, lightSourceObj2.position.z + 0},
        .rotation = {0,0,90},
        .scale = {2,2,2}
    };
    objects[13] = (renderObject) {.transform = transform13, .data = lampeInnenData, .vao = lampeInnenVAO};

    // Lampe Außen rechts
    gameObject transform14 =
    {
        .position = (vec3){lightSourceObj2.position.x + 0.62f, lightSourceObj2.position.y + 0, lightSourceObj2.position.z + 0},
        .rotation = {0,0,90},
        .scale = {2,2,2}
    };
    objects[14] = (renderObject) {.transform = transform14, .data = lampeAussenData, .vao = lampeAussenVAO};

    // Lampe Innen links
    gameObject transform15 =
    {
        .position = (vec3){lightSourceObj1.position.x - 0.62f, lightSourceObj1.position.y + 0, lightSourceObj1.position.z + 0},
        .rotation = {0,0,-90},
        .scale = {2,2,2}
    };
    objects[15] = (renderObject) {.transform = transform15, .data = lampeInnenData, .vao = lampeInnenVAO};

    // Lampe Außen links
    gameObject transform16 =
    {
        .position = (vec3){lightSourceObj1.position.x - 0.62f, lightSourceObj1.position.y + 0, lightSourceObj1.position.z + 0},
        .rotation = {0,0,-90},
        .scale = {2,2,2}
    };
    objects[16] = (renderObject) {.transform = transform16, .data = lampeAussenData, .vao = lampeAussenVAO};

    // Brett Außen
    gameObject transform17 =
    {
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {0, 90, 0},
        .scale = {1, 1, 1}
    };
    objects[17] = (renderObject) {.transform = transform17, .data = brettAussenData, .vao = brettAussenVAO};

    // Brett Oben
    gameObject transform18 =
    {
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {0, 90, 0},
        .scale = {1, 1, 1}
    };
    objects[18] = (renderObject) {.transform = transform18, .data = brettObenData, .vao = brettObenVAO};

    // Weiße Figuren
    gameObject transform19 =
    {
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {0, 0, 0},
        .scale = {1, 1, 1}
    };
    objects[19] = (renderObject) {.transform = transform19, .data = whiteData, .vao = whiteVAO};

    // Schwarze Figuren
    gameObject transform20 =
    {
        .position = {0.0f, 0.0f, 0.0f},
        .rotation = {0, 0, 0},
        .scale = {1, 1, 1}
    };
    objects[20] = (renderObject) {.transform = transform20, .data = blackData, .vao = blackVAO};

    objectCount = 21; // Anzahl der Objekte
}

// Zeichnungsfunktion
void draw(void)
{
    GLfloat timeValue = glfwGetTime() / 2.0f; // Zeitwert für Animationen

    // Rotieren der Ventilatorblätter
    objects[11].transform.rotation.y = timeValue * 200; 

    // Berechnung der Kameraposition und -rotation basierend auf der Zeit
    camera.transform.position = (vec3) {4.5 * sinf(timeValue / 8.0f), 5.0f, 4.5 * cosf(timeValue / 8.0f)};
    camera.transform.rotation = (vec3) {-sinf(timeValue / 8.0f), 0.0f, -cosf(timeValue / 8.0f)};

    // Hauptcontainer für Objekte
    gameObject containerTransform =
    {
        .position = {0,0,0},
        .rotation = {0,0,0},
        .scale = {1,1,1}
    };

    // Untercontainer für die ersten 3 Objekte
    objectContainer subContainer = (objectContainer)
    {
        .containers = NULL,
        .containerCount = 0,
        .objects = objects,
        .objectCount = 3,
        .transform = containerTransform
    };

    // Hauptszene mit den restlichen Objekten
    gameObject containerTransform2 =
    {
        .position = {0,0,0},
        .rotation = {0,0,0},
        .scale = {1,1,1}
    };

    objectContainer scene = (objectContainer)
    {
        .containers = &subContainer,
        .containerCount = 1,
        .objects = (objects + 3),
        .objectCount = objectCount - 3,
        .transform = containerTransform2
    };

    // Lichtquellen-Array
    lightObject* lights[2];
    lights[0] = &lightSourceObj1;
    lights[1] = &lightSourceObj2;

    // Framebuffer-Objekte
    static GLuint renderTexture = 0;
    static GLuint framebuffer = 0, renderbuffer = 0;

    // Framebuffer für die Spiegelreflexion erstellen
    createFramebuffer(&framebuffer, &renderbuffer, &renderTexture, 1024, 1024); 

    // Spiegelkamera berechnen
    calculateMirrorCamera(&objects[9]);

    // Szene in den Framebuffer zeichnen
    drawScene(&scene, &mirrorCamera, lights, program, program2, 1024, 1024, framebuffer, renderTexture, GL_TRUE, lightOffset); 

    // Textur des Spiegels setzen
    objects[9].data.material.texture = renderTexture; 

    // Szene in das Fenster zeichnen
    drawScene(&scene, &camera, lights, program, program2, width, height, 0, 0, GL_FALSE, lightOffset); 
}

// Callback-Funktion für die Größenänderung des Framebuffers
void framebufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight) 
{
    aspectRatio = newWidth / (GLfloat)newHeight; // Seitenverhältnis neu berechnen
    width = newWidth; // Neue Breite setzen
    height = newHeight; // Neue Höhe setzen
    camera.aspectRatio = aspectRatio; // Seitenverhältnis der Kamera setzen

    glViewport(0, 0, width, height); // Viewport neu setzen
}

double timeDiff;

// Funktion zur Anzeige der FPS
void displayFPS(GLFWwindow* window, int updateFrequency)
{
    static int frameCount;
    static double prevTime = 0.0f;
    static char str[32]; 

    // Zeitdifferenz und Framezähler aktualisieren
    timeDiff = glfwGetTime() - prevTime;
    frameCount++;

    // Wenn die Zeitdifferenz größer ist als der Aktualisierungszeitraum, FPS und Framezeit berechnen und anzeigen
    if (timeDiff >= 1.0 / updateFrequency) 
    {
        snprintf(str, 32, "%.1f fps | %.3f ms", (1.0 / timeDiff) * frameCount, (timeDiff / frameCount) * 1000.0); 
        glfwSetWindowTitle(window, str); // Fenster-Titel setzen
        prevTime = timeDiff + prevTime; // Zeitstempel aktualisieren
        frameCount = 0; // Framezähler zurücksetzen
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Überprüfen, ob die "Pfeil nach oben" Taste gedrückt wird
    if (key == GLFW_KEY_UP)
    {
        // Erhöhe den Lichtoffset basierend auf der vergangenen Zeit (timeDiff)
        lightOffset += timeDiff/3.0f;
    }

    // Überprüfen, ob die "Pfeil nach unten" Taste gedrückt wird
    if (key == GLFW_KEY_DOWN)
    {
        // Verringere den Lichtoffset basierend auf der vergangenen Zeit (timeDiff)
        lightOffset -= timeDiff/3.0f;
    }

    // Begrenze den Lichtoffset auf ein Minimum von -0.3f
    if (lightOffset < -0.3f)
    {
        lightOffset = -0.3f;
    } 

    // Begrenze den Lichtoffset auf ein Maximum von 0.9f
    if (lightOffset > 0.9f)
    {
        lightOffset = 0.9f;
    } 
}

// Hauptfunktion
int main(void)
{
    glfwInit(); // GLFW initialisieren

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL-Version 3.3 anfordern
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Fenster erstellen
    GLFWwindow* window = glfwCreateWindow(START_WINDOW_WIDTH, START_WINDOW_HEIGHT, "", NULL, NULL); 

    // Fehlerprüfung beim Erstellen des Fensters
    if (window == NULL) 
    {
        printf("Failed to create window\n"); 
        glfwTerminate(); // GLFW beenden
        return -1;
    }

    // Callback-Funktion für die Größenänderung setzen
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); 
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window); // Kontext des Fensters setzen

    glewInit(); // GLEW initialisieren

    init(); // Initialisierung durchführen

    // Initiale Größe setzen
    framebufferSizeCallback(window, START_WINDOW_WIDTH, START_WINDOW_HEIGHT); 

    // Hauptschleife
    while (!glfwWindowShouldClose(window))
    {
        displayFPS(window, 2); // FPS anzeigen
        draw(); // Szene zeichnen
        glfwSwapBuffers(window); // Buffer tauschen
        glfwPollEvents(); // Ereignisse abfragen
    }

    glfwTerminate(); // GLFW beenden

    return 0; // Programm beenden
}
