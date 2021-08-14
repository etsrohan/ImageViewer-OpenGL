# ---Makefile---

#target: dependencies
#	action

all: image_display_all image_display

image_display: image_display.o glad.o stb_image.o functions.o
	gcc image_display.o glad.o stb_image.o functions.o -o image_display -lglfw3 -lgdi32 -lopengl32 -std=c99 -pedantic -pipe -Wall -Wextra
image_display_all: image_display_all.o glad.o stb_image.o functions.o
	gcc image_display_all.o glad.o stb_image.o functions.o -o image_display_all -lglfw3 -lgdi32 -lopengl32 -std=c99 -pedantic -pipe -Wall -Wextra
image_display.o: image_display.c
	gcc -c image_display.c
glad.o: src/glad.c
	gcc -c src/glad.c
stb_image.o: src/stb_image.c
	gcc -c src/stb_image.c
functions.o: src/functions.c
	gcc -c src/functions.c
image_display_all.o: image_display_all.c
	gcc -c image_display_all.c
