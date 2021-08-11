// Headers and Libraries
#include "glad/include/glad/glad.h"
#define GLF_INCLUDE_NONE
#include "glfw-3.3.4.bin.WIN32/include/GLFW/glfw3.h"

#include "stb_image.h" //stbi_load, stbi_image_free

#include <stdio.h>
#include <stdlib.h>
#include <math.h> //sin()
#include <string.h> //memcpy

// Shader struct
typedef struct {
    unsigned int name;
    const char* src_code;
} MyShader;

// Function Declarations
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
char* read_source_code(char* filename);
uint8_t* read_YUV_image(char* filename, int width, int height, int select);
void texture_YUV_420(uint8_t* data, uint8_t* y_plane, uint8_t* u_plane, uint8_t* v_plane, unsigned int *texture1, unsigned int *texture2, unsigned int *texture3, int img_width, int img_height);
void texture_YUV_422(uint8_t* data, uint8_t* y_plane, uint8_t* u_plane, uint8_t* v_plane, unsigned int *texture1, unsigned int *texture2, unsigned int *texture3, int img_width, int img_height);
void texture_RGB(uint8_t* data, unsigned int *texture1, int img_width, int img_height);

// Static Variables
static const float vertices[] = { //Array of vertices coordinates in 3D
   -1.0f, -1.0f,  0.0f,      0.0f, 0.0f,//1, Bottom Left
    1.0f, -1.0f,  0.0f,      1.0f, 0.0f,//2, Bottom Right
    1.0f,  1.0f,  0.0f,      1.0f, 1.0f,//3, Top Right
   -1.0f,  1.0f,  0.0f,      0.0f, 1.0f //4, Top Left
};
static const unsigned int indices[] = { // Array of indices for setting up 2 triangles
    0, 1, 2, //Triangle 1
    0, 2, 3  //Triangle 2
};

static int wireframe = 1; // a boolean to keep track of wireframe mode or fill mode

int main(void){
    // Initializing Variables
    GLFWwindow *window;
    unsigned int VBO, VAO, EBO;
    // VBO = Vertex Buffer Object, VAO = Vertex Array Object, EBO = Element Buffer Object
    unsigned int shaderProgram;
    MyShader  vertexShader, fragmentShader;
    char* dummy1, * dummy2;
    // Initialize Texture variables
    uint8_t *data;
    uint8_t *y_plane = NULL, *u_plane = NULL, *v_plane = NULL;
    unsigned int texture1, texture2, texture3;

    //====================================================================================
    // USER INPUT DATA IMAGE WIDTH/HEIGHT, SELECT AND IMAGE NAME WITH .YUV/.UYVY EXTENSION
    int img_width = 1920, img_height = 1080, number_channels = 0;  // This is important to predefine for YUV formats, not so much RGB
    const int select = 1; // Select format 1: YUV420 image, 2: YUV422 (uyvy) image, 3: RGB image
    char* my_image = "jiraya_1920_1080.yuv";
    //====================================================================================

    // Initialize src code for shaders
    dummy1 = read_source_code("shaders/vertex_YUV.shader");
    if(select == 3)
        dummy2 = read_source_code("shaders/fragment_RGB.shader");
    else if(select == 1 || select == 2)
        dummy2 = read_source_code("shaders/fragment_YUV.shader");
    vertexShader.src_code = dummy1;
    fragmentShader.src_code = dummy2;


    // Initialize GLFW
    if (!glfwInit()){
        printf("Couldn't initiate GLFW\n");
        return 1;
    }

    // Initialize Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(800, 600, "Image Loader", NULL, NULL);
    if (!window){
        printf("Couldn't open window\n");
        glfwTerminate();
        return 1;
    }

    // Make Context Current
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Create/Compile Vertex Shader by giving Vertex Shader source code
    vertexShader.name = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader.name, 1, &vertexShader.src_code, NULL);
    glCompileShader(vertexShader.name);

    // Create/Compile Fragment Shader by giving Fragment Shader source code
    fragmentShader.name = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader.name, 1, &fragmentShader.src_code, NULL);
    glCompileShader(fragmentShader.name);

    // Create/Link shaders to program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader.name);
    glAttachShader(shaderProgram, fragmentShader.name);
    glLinkProgram(shaderProgram);

    // delete unnecessary shaders
    glDeleteShader(vertexShader.name);
    glDeleteShader(fragmentShader.name);
    
    // Generate VBO AND VAO and EBO
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    // Bind vertex array and vertex buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind Element Buffer Object and copy data to buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Taking Position input from object into buffer 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbinding VAO and VBO (not necessary but to be safe)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Opening and setting YUV file and parameters
    if(select == 1 || select == 2){
        data = read_YUV_image(my_image, img_width, img_height, select);

        if(data){
            if(select == 1) // selecting YUV420 image
                texture_YUV_420(data, y_plane, u_plane, v_plane, &texture1, &texture2, &texture3, img_width, img_height);
            if(select == 2)
                texture_YUV_422(data, y_plane, u_plane, v_plane, &texture1, &texture2, &texture3, img_width, img_height);
        }
        else printf("Failed to load texture\n");
    }
    else if(select == 3){
        data = stbi_load(my_image, &img_width, &img_height, &number_channels, 0);
        if(data){
            texture_RGB(data, &texture1, img_width, img_height);
        }
        else printf("Failed to load texture\n");
    }

    // Give fragment shader their respective textures
    glUseProgram(shaderProgram);
    if(select == 1 || select == 2){
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture3"), 2);
    }
    else if(select == 3)
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    

    free(data);

    // Input Callback
    glfwSetKeyCallback(window, keyCallback);

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
        if(select == 1 || select == 2){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, texture3);
        }
        else if(select == 3){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
        }

        // Set shader program
        glUseProgram(shaderProgram);
        
        // Render 2 triangles
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Switch Front and Back Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Delete buffers to free memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    free(dummy1);
    free(dummy2);
    // End glfw api
    glfwTerminate();
    return 0;
}

// Function Definitions
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    /*This function takes user keyboard input and:
        if key press = Esc -> close window
        if key press = Space -> toggle between solid and wireframe mode*/

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        if (wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframe = 0;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframe = 1;
        }
    }
}

char* read_source_code(char* filename){
    /*This function takes in the file path from input and sends back the source
    code for the specified shader*/
    char* buffer = NULL;
    long length = 0;
    FILE * file_pointer = NULL;
    int i = 0;

    file_pointer = fopen(filename, "r");

    if (file_pointer){
        fseek(file_pointer, 0, SEEK_END);
        length = ftell(file_pointer);
        fseek(file_pointer, 0, SEEK_SET);
        buffer = malloc(sizeof(char) * (length + 1));

        buffer[length] = '\0'; // This was not working (I was getting garbage values after program ended.)
        fread(buffer, 1, length, file_pointer);
        fclose(file_pointer);
    }


    // Go through file backwards and whereever it finds '//' replace first '/' with '\0'
    for(i = length - 1; i > 0; i--){
        if(buffer[i] == '/' && buffer[i + 1] == '/'){
            buffer[i] = '\0';
            break;
        }
    }

    //printf(buffer);
    //printf("\n");
    return buffer;
}

uint8_t* read_YUV_image(char* filename, int width, int height, int select){
    /*This function takes in a YUV format (Raw) and loads it with its 3 components, 
    luminance (Y component), chrominanceU (U component), chrominanceV (V component)
    and returns a single pointer
    select = 1 : YUV420 format, select = 2 : UYVY (YUV422) format
    Note: This function does require that the user be aware of the width and height 
    of the image*/

    FILE* file_pointer = NULL;
    uint8_t* data = NULL;

    file_pointer = fopen(filename, "rb");

    if(file_pointer){
        if(select == 1){
            data = calloc(width * height * 3 / 2 + 1, sizeof(uint8_t));
            fseek(file_pointer, 0, SEEK_SET);
            fread(data, sizeof(uint8_t), width * height * 3 / 2, file_pointer);
            data[width * height * 3 / 2] = '\0';
        }
        if(select == 2){
            data = calloc(width * height * 2 + 1, sizeof(uint8_t));
            fseek(file_pointer, 0, SEEK_SET);
            fread(data, sizeof(uint8_t), width * height * 2, file_pointer);
            data[width * height * 2] = '\0';
        }
        fclose(file_pointer);
    }
    else{
        printf("FILE FAILED TO LOAD\n");
    }
    return data;
}

void texture_YUV_420(uint8_t* data, uint8_t* y_plane, uint8_t* u_plane, uint8_t* v_plane, unsigned int *texture1, unsigned int *texture2, unsigned int *texture3, int img_width, int img_height){
    /* This function sets up the textures for the YUV420 (yuv) image file by setting up the y,u and v_plane arrays and binding them to 3 individual single channel textures.*/
    // Setting up y/u/v_plane to pass to shader
    y_plane = data;
    u_plane = data + img_width * img_height;
    v_plane = u_plane + (img_width * img_height / 4);

    // Send the plane values to shader as separate shaders
        // Y Values
    glGenTextures(1, texture1);
    glBindTexture(GL_TEXTURE_2D, *texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(y_plane){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img_width, img_height, 0, GL_RED, GL_UNSIGNED_BYTE, y_plane); 
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("Y values failed to load\n");
        // U Values
    glGenTextures(1, texture2);
    glBindTexture(GL_TEXTURE_2D, *texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(u_plane){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img_width / 2, img_height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, u_plane);  // width/2 and height/2 because the u component corresponds to 4 components in y (as a box)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("U values failed to load\n");

        // V Values
    glGenTextures(1, texture3);
    glBindTexture(GL_TEXTURE_2D, *texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(v_plane){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img_width / 2, img_height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, v_plane); // same as u component, (Imagine y, u and v as co-centeric boxes but u and v have half the dimensions of y)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("V values failed to load\n");
}

void texture_YUV_422(uint8_t* data, uint8_t* y_plane, uint8_t* u_plane, uint8_t* v_plane, unsigned int *texture1, unsigned int *texture2, unsigned int *texture3, int img_width, int img_height){
    /* This function sets up the textures for the YUV422 (uyvy) image file by setting up the y,u and v_plane arrays and binding them to 3 individual single channel textures.*/
    // Setting up plane data
    y_plane = calloc(img_width * img_height, sizeof(uint8_t));
    u_plane = calloc(img_width * img_height / 2, sizeof(uint8_t));
    v_plane = calloc(img_width * img_height / 2, sizeof(uint8_t));

    // looping through each block of 4 bytes in data to assign y,u,v_plane data
    for(int i = 0; i < img_width * img_height / 2; i++){
        u_plane[i]          = data[4 * i];
        y_plane[2 * i]      = data[4 * i + 1];
        v_plane[i]          = data[4 * i + 2];
        y_plane[2 * i + 1]  = data[4 * i + 3];
    }

    
    // Send the plane values to shader as separate shaders
        // Y Values
    glGenTextures(1, texture1);
    glBindTexture(GL_TEXTURE_2D, *texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(y_plane){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img_width, img_height, 0, GL_RED, GL_UNSIGNED_BYTE, y_plane);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("Y values failed to load\n");
        // U Values
    glGenTextures(1, texture2);
    glBindTexture(GL_TEXTURE_2D, *texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(u_plane){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img_width / 2, img_height, 0, GL_RED, GL_UNSIGNED_BYTE, u_plane); // width/2 because the u component corresponds to 2 components in y (horizontally)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("U values failed to load\n");

        // V Values
    glGenTextures(1, texture3);
    glBindTexture(GL_TEXTURE_2D, *texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(v_plane){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img_width / 2, img_height, 0, GL_RED, GL_UNSIGNED_BYTE, v_plane); // same as u component, (Imagine y, u and v as transposed onto each other but u and v are only half as wide with same center)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("V values failed to load\n");

    free(y_plane);
    free(u_plane);
    free(v_plane);
}

void texture_RGB(uint8_t* data, unsigned int *texture1, int img_width, int img_height){
    glGenTextures(1, texture1);
    glBindTexture(GL_TEXTURE_2D, *texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(data){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("Y values failed to load\n");
}