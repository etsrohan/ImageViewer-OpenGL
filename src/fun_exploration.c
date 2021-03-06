// Headers and Libraries
#include "header/glad.h"
#define GLF_INCLUDE_NONE
#include "header/glfw3.h"

#include "header/stb_image.h" //stbi_load

#include <stdio.h>
#include <stdlib.h>
#include <math.h> //sin()

// Shader struct
typedef struct {
    unsigned int name;
    const char* src_code;
} MyShader;

// Function Declarations
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
char* read_source_code(char* filename);

// Static Variables
static const float vertices[] = { //Array of vertices coordinates in 3D
   -1.0f, -1.0f, 0.0f,      1.0f, 0.0f, 0.0f,       0.0f, 0.0f,//1, Bottom Left
    1.0f, -1.0f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f,//2, Bottom Right
    1.0f,  1.0f, 0.0f,      1.0f, 0.0f, 0.0f,       1.0f, 1.0f,//3, Top Right
   -1.0f,  1.0f, 0.0f,      0.0f, 0.0f, 1.0f,       0.0f, 1.0f //4, Top Left
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
    int img_width, img_height, nrChannels;
    unsigned char * data;
    unsigned int texture1, texture2; 

    // Initialize src code for shaders
    dummy1 = read_source_code("src/shaders/vertex.shader");
    dummy2 = read_source_code("src/shaders/fragment.shader");
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
    window = glfwCreateWindow(1280, 720, "Image Loader", NULL, NULL);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbinding VAO and VBO (not necessary but to be safe)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // TEXTURE 1
    // load and create texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set texture parameters
        //wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create texture from loaded image
    stbi_set_flip_vertically_on_load(1); // Setting image to load right side up
    data = stbi_load("rinnegan.png", &img_width, & img_height, &nrChannels, 0);
    if(data){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    // TEXTURE 2
    // load and create texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create texture from loaded image
    stbi_set_flip_vertically_on_load(1); // Setting image to load right side up
    data = stbi_load("jiraya.jpg", &img_width, & img_height, &nrChannels, 0);
    if(data){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    // Give fragment shaders their respective textures
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Putting in sine gamma for 1st texture
        GLfloat timeValue = glfwGetTime();
        GLfloat gamma = (sin(timeValue) / 2) + 0.5;
        glUniform1f(glGetUniformLocation(shaderProgram, "gamma"), gamma);

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