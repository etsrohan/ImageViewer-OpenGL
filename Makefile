# ---Makefile---

#target: dependencies
#	action

all: fun_exploration image_display

image_display: image_display.o glad.o stb_image.o functions.o
	gcc image_display.o glad.o stb_image.o functions.o -o image_display -lglfw3 -lgdi32 -lopengl32 -std=c99 -pedantic -pipe -Wall -Wextra
image_display.o: image_display.c
	gcc -c image_display.c
glad.o: src/glad.c
	gcc -c src/glad.c
stb_image.o: src/stb_image.c
	gcc -c src/stb_image.c
functions.o: src/functions.c
	gcc -c src/functions.c
fun_exploration: fun_exploration.o glad.o stb_image.o
	gcc fun_exploration.o glad.o stb_image.o -o fun_wallpaper -lglfw3 -lgdi32 -lopengl32 -std=c99 -pedantic -pipe -Wall -Wextra
fun_exploration.o: fun_exploration.c
	gcc -c fun_exploration.c