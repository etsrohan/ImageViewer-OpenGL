// Headers and Libraries
#include "glad/include/glad/glad.h"
#define GLF_INCLUDE_NONE
#include "glfw-3.3.4.bin.WIN32/include/GLFW/glfw3.h"

#include <stdio.h>
#include <stdlib.h>

// Function Declarations
static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

// Static Variables
static const float vertices[] = { //Array of vertices coordinates in 3D
   -0.5f, -0.5f, 0.0f, //1, Bottom Left
    0.5f, -0.5f, 0.0f, //2, Bottom Right
    0.5f,  0.5f, 0.0f, //3, Top Right
   -0.5f,  0.5f, 0.0f  //4, Top Left
};
static const unsigned int indices[] = { // Array of indices for setting up 2 triangles
    0, 1, 2, //Triangle 1
    0, 2, 3  //Triangle 2
};

static const char* vertex_shader_src = // Vertex Shader source code
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

static const char* fragment_shader_src = // Fragment Shader source code
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

static int wireframe = 1; // a boolean to keep track of wireframe mode or fill mode

int main(void){
    // Initializing Variables
    GLFWwindow *window;
    unsigned int VBO, VAO, EBO;
    // VBO = Vertex Buffer Object, VAO = Vertex Array Object, EBO = Element Buffer Object
    unsigned int vertexShader, fragmentShader, shaderProgram;


    // Initialize GLFW
    if (!glfwInit()){
        printf("Couldn't initiate GLFW\n");
        return 1;
    }

    // Initialize Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Image Loader", NULL, NULL);
    if (!window){
        printf("Couldn't open window\n");
        glfwTerminate();
        return 1;
    }

    // Make Context Current
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Create/Compile Vertex Shader by giving Vertex Shader source code
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertexShader);

    // Create/Compile Fragment Shader by giving Fragment Shader source code
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragmentShader);

    // Create/Link shaders to program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // delete unnecessary shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Unbinding VAO and VBO (not necessary but to be safe)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

        // Render 2 triangles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Switch Front and Back Buffers
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    // Delete buffers to free memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
 
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