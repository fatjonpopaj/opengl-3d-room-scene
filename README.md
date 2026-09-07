# Projektarbeit

### 1. Projektübersicht
Das Projekt konzentriert sich auf die Erstellung einer Computergrafik-Szene unter Verwendung von OpenGL. Die Szene umfasst einen Raum mit detaillierter Beleuchtung, die die Atmosphäre hervorhebt. Eine dynamische Kamera ermöglicht es, den Raum aus verschiedenen Blickwinkeln zu betrachten, insbesondere durch eine rechtsdrehende Bewegung. Zentral in der Szene platziert sind ein Tisch und zwei Stühle, die die Struktur und die räumliche Tiefe ergänzen. Die Beleuchtung setzt Akzente auf die Szenerie, indem sie Schatteneffekte erzeugt und so eine realistische und immersive Umgebung schafft.

### 2. Installation
Entwicklungsumgebung vorbereiten:

   - Stelle sicher, dass Visual Studio Code (VSCode) oder CLion auf deinem System installiert ist, da wir diese IDEs für die Entwicklung verwendet haben.

OpenGL-Bibliotheken einrichten:

   - Da wir OpenGL für die Grafikprogrammierung genutzt haben, sind die GLFW- und GLEW-Bibliotheken erforderlich.
        - GLFW (OpenGL Framework):
           - Installiere GLFW entsprechend der Anleitung auf der offiziellen GLFW-Website oder über ein Paketverwaltungssystem wie Homebrew (für macOS) oder apt-get (für Linux).
        - GLEW (The OpenGL Extension Wrangler Library):
           - Lade GLEW von der GLEW-Website herunter oder installiere es über ein Paketverwaltungssystem.

### 3. Verwendung
   #### Bauen und Starten
   - make progMacx64 (für Mac-Systeme mit einer x64-Architektur)
   - make progMacARM (für Mac-Systeme mit ARM-Architektur)
   - make progWin.exe (für Windows-Systeme)
   - make progLinux (für Linux-Systeme)

   #### Interaktion
   - Um mit der Szene zu interagieren, kann man die Pfeiltasten nach oben und unten benutzen, diese passen die Intensität der Lichter an.

### 4. Dateistruktur
   - Die Ordnerstruktur des Projekts "Projektarbeit" ist organisiert und strukturiert, um alle Quellcodedateien, Ressourcen und Hilfsdateien übersichtlich zu verwalten und die Entwicklung eines umfangreichen Softwareprojekts zu unterstützen.

    -Projektarbeit
        -src
           -obj
               -boden.obj
               -brettAussen.obj
               -...
           -shader
               -fragmentShader.glsl
               -...
           -tex
               -boden.jpg
               -...
           -Makefile
           -fileManager.c
           -fileManager.h
           -main.c
           -mat.c
           -mat.h
           -objStructs.h
           -renderer.c
           -renderer.h
           -transformations.c
           -transformations.h
           -vec.c
           -vec.h
         -test
           -test.c
           -Makefile

            
- Hier eine kurze Beschreibung der Hauptordner und Dateien:
   - src: Dies ist der Hauptordner, der alle Quellcodedateien und Ressourcen für Ihr Projekt enthält.
   - obj: Enthält die Dateien für 3D-Objekte, z. B. boden.obj, brettAussen.obj, usw.
   - shader: Hier liegen Ihre Shader-Programme, z. B. fragmentShader.glsl.
   - tex: Dieser Ordner enthält Texturen, z. B. boden.jpg.
   - Makefile: Das Makefile für die Kompilierung und den Build-Prozess.
   - fileManager.c, fileManager.h: Implementierung und Header-Datei für den Datei-Manager.
   - main.c: Die Hauptdatei, die die main()-Funktion und die Hauptlogik des Programms enthält.
   - mat.c, mat.h: Implementierung und Header-Datei für Matrix-Operationen.
   - objStructs.h: Header-Datei, die Strukturen für Objekte definiert.
   - renderer.c, renderer.h: Implementierung und Header-Datei für den Renderer.
   - transformations.c, transformations.h: Implementierung und Header-Datei für Transformationen (z. B. Rotation, Skalierung).
   - vec.c, vec.h: Implementierung und Header-Datei für Vektor-Operationen.

### 5. Funktionen und Features
   - Hier sind ausgewählte Zeilen als Beispielcode aus den wichtigsten Funktionen und Features des gegebenen Programms. Diese Zeilen veranschaulichen Schlüsselelemente wie das Zeichnen der Szene, das Verwalten von Framebuffers für Spiegelreflexionen, die Anzeige von FPS, die Größenänderung des Framebuffers und die Organisation von Objekten in Containern. Jede Zeile bietet einen Einblick in spezifische Funktionalitäten des Programms.

      - Szene zeichnen (draw Funktion):
          - Rotieren der Ventilatorblätter basierend auf der Zeit:
          -     objects[11].transform.rotation.y = timeValue * 50;

      - Framebuffer-Verwaltung und Spiegelreflexion (createFramebuffer Funktion):
          - Erstellen eines Framebuffers für die Spiegelreflexion:
          -     createFramebuffer(&framebuffer, &renderbuffer, &renderTexture, 1024, 1024);
       
      - FPS-Anzeige (displayFPS Funktion):
          - Setzen des Fenstertitels mit FPS und durchschnittlicher Framezeit:
          -     snprintf(str, 32, "%.1f fps | %.3f ms", (1.0 / timeDiff) * frameCount, (timeDiff / frameCount) *                     1000.0);
                glfwSetWindowTitle(window, str);

      - Größenänderung des Framebuffers (framebufferSizeCallback Funktion):
          - Aktualisieren des Viewports und des Seitenverhältnisses der Kamera:
          -     aspectRatio = newWidth / (GLfloat)newHeight;
                camera.aspectRatio = aspectRatio;
                glViewport(0, 0, width, height);

      - Objektcontainer für effiziente Szenenorganisation (objectContainer Struktur):
          - Initialisierung eines Objektcontainers für die Hauptszene:
          -     objectContainer scene = {
                .containers = &subContainer,
                .containerCount = 1,
                .objects = (objects + 3),
                .objectCount = objectCount - 3,
                .transform = containerTransform2
                };

### Texturen
   - stuhlTexture: Verwendet für den Stuhl.
   - tischTexture: Verwendet für den Tisch.
   - bodenTexture: Verwendet für den Boden.
   - wandTexture: Verwendet für die Wände.
   - mirrorTexture: Verwendet für den Spiegel.
   - ventilatorTexture: Verwendet für den Ventilator.
   - lampeInnenTexture: Verwendet für die Lampe innen.
   - lampeAussenTexture: Verwendet für die Lampe außen.
   - brettAussenTexture: Verwendet für das äußere Brett.
   - brettObenTexture: Verwendet für das obere Brett.
   - whiteTexture: Verwendet für die weißen Figuren.
   - blackTexture: Verwendet für die schwarzen Figuren.

## Quellen
### Objekte
#### schwarzeFiguren.obj, weisseFiguren.obj
- https://github.com/ericdude4/chess
#### ventilatorBasis.obj, ventilatorBlaetter.obj, ventilatorStange.obj
- https://www.turbosquid.com/de/3d-models/3d-fan-2219173
### Texturen
#### boden.jpg
- https://www.sketchuptextureclub.com/textures/architecture/concrete/plates/clean/concrete-clean-plates-wall-texture-seamless-01717
#### schwarzeFiguren.jpg
- https://www.sketchuptextureclub.com/textures/architecture/wood/fine-wood/dark-wood/natural-wenge-pbr-texture-seamless-22006
#### ventilatorBasis.jpg
- https://www.sketchuptextureclub.com/textures/materials/metals/brushed-metals/circular-brushed-aluminium-metal-texture-09896
#### ventilatorBlaetter.jpg
- https://www.sketchuptextureclub.com/textures/materials/metals/basic-metals/aluminium-metal-surface-texture-seamless-09792
#### weisseFiguren.jpg
- https://www.sketchuptextureclub.com/textures/architecture/wood/fine-wood/light-wood/natural-light-wood-fine-texture-seamless-04291
#### wood.png
- https://www.sketchuptextureclub.com/textures/architecture/wood/fine-wood/dark-wood/dark-fine-wood-texture-seamless-04274
