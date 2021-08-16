#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>

//#define GLFW_DLL
#define _GLFW_BUILD_DLL
#define GLFW_INCLUDE_NONE
#include "glfw-3.3.4.bin.WIN32\include\GLFW\glfw3.h"
#include "glad\include\glad\glad.h"


static GLFWwindow* window;

static double last = 0.0;
static double now = 0.0;
static float delta = 1.0f;

int Window_init(int width, int height, char* title);
void Window_update(void (*update)());
int Window_shouldClose();
void Window_destroy();

//
// IMPLEMENTATION
//

int Window_init(int width, int height, char* title){
    if (!glfwInit()){
        return 0;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window){
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    */


    return 1;

}

void Window_update(void (*update)()){
    last = glfwGetTime();

    (*update)(delta);

    glfwSwapBuffers(window);
    glfwPollEvents();

    now = glfwGetTime();
    delta = (float)(now - last) * 10.0f;
}

int Window_shouldClose(){
    return glfwWindowShouldClose(window);
}
void Window_destroy(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

#endif
