# ---Makefile---

#target: dependencies
#	action

all: main image_display

image_display: image_display.o glad.o stb_image.o
	gcc image_display.o glad.o stb_image.o -o image_display -lglfw3 -lgdi32 -lopengl32 -std=c99 -pedantic -pipe -Wall -Wextra
image_display.o: image_display.c
	gcc -c image_display.c
glad.o: glad/src/glad.c
	gcc -c glad/src/glad.c
stb_image.o: stb_image.c
	gcc -c stb_image.c
main: main.o glad.o stb_image.o
	gcc main.o glad.o stb_image.o -o fun_wallpaper -lglfw3 -lgdi32 -lopengl32 -std=c99 -pedantic -pipe -Wall -Wextra
main.o: main.c
	gcc -c main.c