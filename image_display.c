// This version is optimized for NVIDIA graphics cards
// Headers and Libraries
#include "src/header/Rohan/glad.h" //gladLoadGL
#define GLF_INCLUDE_NONE
#include "src/header/Rohan/glfw3.h" //glfwFunctions

#include "src/header/Rohan/stb_image.h" //stbi_load, stbi_image_free
#include "src/header/Rohan/functions.h" //most of it

#include <stdio.h> //printf
#include <stdlib.h> //fopen, fread, fseek
#include <string.h> //memcpy

// Single function declaration because this one was giving errors when put into functions.h/.c because it is a static function.
static void rohan_key_callback(ROHAN_REF GLFWwindow* const window, ROHAN_IN int const key, ROHAN_IN int const scancode, ROHAN_IN int const action, ROHAN_IN int const mods);

// MAIN //
int main(ROHAN_NOARGS void){
    // Initializing Variables
    GLFWwindow *window;
    // Initialize Texture variables
    uint8_t *data;
    uint8_t *y_plane = NULL, *u_plane = NULL, *v_plane = NULL;
    unsigned int texture_1, texture_2, texture_3;
    int main_err = EXIT_SUCCESS;
    // FPS Counter Variables
    double last_time = 0;
    double current_time = 0;
    double num_frames = 0;

    //====================================================================================
    // IMAGE INPUT DATA (IMAGE WIDTH/HEIGHT, SELECT AND IMAGE NAME WITH .YUV/.UYVY EXTENSION)
    int img_width = 1920, img_height = 1080, number_channels = 0;  // This is important to predefine for YUV formats, not so much RGB
    int const select = 2; // Select format 1: YUV420 image, 2: YUV422 (uyvy) image, 3: RGB image
    char* my_image = "jiraya_1920_1080.uyvy";
    //====================================================================================

    // Initialize GLFW
    if (!glfwInit()){
        printf("ERROR: Couldn't initiate GLFW\n");
        return ROHAN_GLFW_INIT_ERR;
    }

    // Initialize Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(1280, 720, "Image Loader", NULL, NULL);
    if (!window){
        printf("ERROR: Couldn't open window\n");
        glfwTerminate();
        return ROHAN_WINDOW_INIT_ERR;
    }

    // Make Context Current
    glfwMakeContextCurrent(window);
    // Initiate Glad
    gladLoadGL();
    
    // Opening and setting YUV/RGB file and parameters
    if(select == 1 || select == 2){
        data = rohan_read_YUV_image(my_image, img_width, img_height, select);

        if(data){
            rohan_texture_YUV_optimized(data, y_plane, u_plane, v_plane, &texture_1, img_width, img_height, select);
        }
        free(data);
    }
    else if(select == 3){
        data = stbi_load(my_image, &img_width, &img_height, &number_channels, 0);
        if(data){
            rohan_texture_RGB(data, &texture_1, img_width, img_height);
        }
        stbi_image_free(data);
    }
    if(!data){
        printf("ERROR: Failed to load texture(s)\n");
        goto error;
    }

    // Input Callback
    glfwSetKeyCallback(window, rohan_key_callback);
    // Setup last_time for time per frame calculation
    last_time = glfwGetTime();

    // Loop While Window Is Open
    while (!glfwWindowShouldClose(window)){ 
        // Clear Background
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Keep setting view as size of window changes
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Bind texture

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_1);
        
        glDrawTextureNV(texture_1, 0,
                0, 0, width, height,0,
                0, 1, 1,     0);

        // Print FPS onto terminal
        rohan_fps_counter(&last_time, &current_time, &num_frames);

        //Switch Front and Back Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    printf("\n");
    goto end;
    error:
    printf("\nOOPS! Something went wrong!\n");
    main_err = EXIT_FAILURE;

    end:
    // End glfw api
    glfwTerminate();
    return main_err;
}

static void rohan_key_callback(ROHAN_REF GLFWwindow* const window, ROHAN_IN int const key, ROHAN_IN int const scancode, ROHAN_IN int const action, ROHAN_IN int const mods){
    /*This function takes user keyboard input and:
        if key press = Esc -> close window
        if key press = Space -> toggle between solid and wireframe mode*/

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}