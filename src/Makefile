#MacOS x64

MAC_x64_INCLUDE_PATH =-I/usr/local/include
MAC_x64_LIBRARY_PATH =-L/usr/local/lib
MAC_x64_LINKER_FLAGS =-framework OpenGL -lglew -lglfw 

progMacx64 : *.c
	gcc $^ -o $@ -Wall -std=c99 -pedantic-errors $(MAC_x64_INCLUDE_PATH) $(MAC_x64_LIBRARY_PATH) $(MAC_x64_LINKER_FLAGS)


#MacOS ARM

MAC_ARM_INCLUDE_PATH =$(MAC_x64_INCLUDE_PATH)
MAC_ARM_LIBRARY_PATH =-L/opt/homebrew/lib
MAC_ARM_LINKER_FLAGS =$(MAC_x64_LINKER_FLAGS)

progMacARM : *.c
	gcc $^ -o $@ -Wall -std=c99 -pedantic-errors $(MAC_ARM_INCLUDE_PATH) $(MAC_ARM_LIBRARY_PATH) $(MAC_ARM_LINKER_FLAGS)


#Windows

WIN_INCLUDE_PATH =
WIN_LIBRARY_PATH =-L/usr/local/lib
WIN_LINKER_FLAGS =-lglew32 -lopengl32 -lglfw3 -lgdi32

progWin.exe: *.c
	gcc $^ -o $@ -Wall -std=c99 -pedantic-errors $(WIN_INCLUDE_PATH) $(WIN_LIBRARY_PATH) $(WIN_LINKER_FLAGS)


#Linux

LINUX_INCLUDE_PATH =
LINUX_LIBRARY_PATH =
LINUX_LINKER_FLAGS =-lOpenGL -lGLEW -lglfw -lm

progLinux: *.c 
	gcc $^ -o $@ -Wall -std=gnu99 -pedantic-errors $(LINUX_INCLUDE_PATH) $(LINUX_LIBRARY_PATH) $(LINUX_LINKER_FLAGS)

