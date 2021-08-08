# ---Makefile---

#target: dependencies
#	action

main: main.o glad.o stb_image.o
	gcc main.o glad.o stb_image.o -o pic_display -lglfw3 -lgdi32 -lopengl32
main.o: main.c
	gcc -c main.c
glad.o: glad/src/glad.c
	gcc -c glad/src/glad.c
stb_image.o: stb_image.c
	gcc -c stb_image.c
