# ---Makefile---

#target: dependencies
#	action

all: main main_YUV420

main_YUV420: main_YUV.o glad.o stb_image.o
	gcc main_YUV.o glad.o stb_image.o -o pic_display_YUV420 -lglfw3 -lgdi32 -lopengl32
main_YUV.o: main_YUV.c
	gcc -c main_YUV.c
glad.o: glad/src/glad.c
	gcc -c glad/src/glad.c
stb_image.o: stb_image.c
	gcc -c stb_image.c
main: main.o glad.o stb_image.o
	gcc main.o glad.o stb_image.o -o pic_display -lglfw3 -lgdi32 -lopengl32
main.o: main.c
	gcc -c main.c