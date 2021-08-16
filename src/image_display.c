// Headers and Libraries
#include "header/glad.h" //gladLoadGL
#define GLF_INCLUDE_NONE
#include "header/glfw3.h" //glfwFunctions

#include "header/stb_image.h" //stbi_load, stbi_image_free
#include "header/Rohan/functions.h" //most of it

#include <stdio.h> //printf
#include <stdlib.h> //fopen, fread, fseek
#include <string.h> //memcpy

// Single function declaration because this one was giving errors when put into functions.h/.c because it is a static function.
static void rohan_key_callback(ROHAN_REF GLFWwindow* const window, ROHAN_IN int const key, ROHAN_IN int const scancode, ROHAN_IN int const action, ROHAN_IN int const mods);

// MAIN //
int main(ROHAN_NOARGS void){
    // Initializing Variables
    GLFWwindow *window;
    unsigned int VBO, VAO, EBO;
    // VBO = Vertex Buffer Object, VAO = Vertex Array Object, EBO = Element Buffer Object
    unsigned int shader_program;
    RohanMyShader  vertex_shader, fragment_shader;
    char* dummy_1 = NULL, * dummy_2 = NULL;
    // Initialize Texture variables
    uint8_t *data;
    uint8_t *y_plane = NULL, *u_plane = NULL, *v_plane = NULL;
    unsigned int texture_1, texture_2, texture_3;
    int main_err = EXIT_SUCCESS;
    // FPS Counter Variables
    double last_time = 0;
    double current_time = 0;
    double num_frames = 0;

    // Vertex and triangle index variables
    float const vertices[] = { //Array of vertices coordinates in 3D
       -1.0f, -1.0f,  0.0f,      0.0f, 0.0f,//1, Bottom Left
        1.0f, -1.0f,  0.0f,      1.0f, 0.0f,//2, Bottom Right
        1.0f,  1.0f,  0.0f,      1.0f, 1.0f,//3, Top Right
       -1.0f,  1.0f,  0.0f,      0.0f, 1.0f //4, Top Left
    };
    unsigned int const indices[] = { // Array of indices for setting up 2 triangles
        0, 1, 2, //Triangle 1
        0, 2, 3  //Triangle 2
    };

    //====================================================================================
    // IMAGE INPUT DATA (IMAGE WIDTH/HEIGHT, SELECT AND IMAGE NAME WITH .YUV/.UYVY EXTENSION)
    int img_width = 1920, img_height = 1080, number_channels = 0;  // This is important to predefine for YUV formats, not so much RGB
    int const select = 3; // Select format 1: YUV420 image, 2: YUV422 (uyvy) image, 3: RGB image
    char* my_image = "images/jiraya.jpg";
    //====================================================================================

    // Initialize src code for shaders
    // Using dummys beacuse compiler was giving off warnings
    dummy_1 = rohan_read_shader_source_code("src/shaders/vertex_YUV.shader");
    if(select == 3)
        dummy_2 = rohan_read_shader_source_code("src/shaders/fragment_RGB.shader");
    else if(select == 1 || select == 2)
        dummy_2 = rohan_read_shader_source_code("src/shaders/fragment_YUV.shader");
    else{
        fprintf(stderr, "\nERROR: Please enter a valid value for 'select' (1, 2 or 3)\n");
        free(dummy_1);
        return EXIT_FAILURE;
    }
    if(dummy_1 == NULL || dummy_2 == NULL){
        fprintf(stderr, "\nERROR: Shader files failed to open or close\n");
        return EXIT_FAILURE;
    }
    vertex_shader.src_code = dummy_1;
    fragment_shader.src_code = dummy_2;

    // Initialize GLFW
    if (!glfwInit()){
        fprintf(stderr, "ERROR: Couldn't initiate GLFW\n");
        return ROHAN_GLFW_INIT_ERR;
    }

    // Initialize Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(1280, 720, "Image Loader", NULL, NULL);
    if (!window){
        fprintf(stderr, "ERROR: Couldn't open window\n");
        glfwTerminate();
        return ROHAN_WINDOW_INIT_ERR;
    }

    // Make Context Current
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // Create/Compile Vertex Shader by giving Vertex Shader source code
    vertex_shader.name = glCreateShader(GL_VERTEX_SHADER);
    if(!vertex_shader.name || vertex_shader.name == GL_INVALID_ENUM){
        fprintf(stderr, "ERROR: Vertex shader failed to be created.\n");
        goto error;
    }
    glShaderSource(vertex_shader.name, 1, &vertex_shader.src_code, NULL);
    glCompileShader(vertex_shader.name);

    // Create/Compile Fragment Shader by giving Fragment Shader source code
    fragment_shader.name = glCreateShader(GL_FRAGMENT_SHADER);
    if(!fragment_shader.name || fragment_shader.name == GL_INVALID_ENUM){
        fprintf(stderr, "ERROR: Fragment shader failed to be created.\n");
        goto error;
    }
    glShaderSource(fragment_shader.name, 1, &fragment_shader.src_code, NULL);
    glCompileShader(fragment_shader.name);

    // Create/Link shaders to program
    shader_program = glCreateProgram();
    if(!shader_program){
        fprintf(stderr, "ERROR: Shader program failed to be created.\n");
        goto error;
    }
    glAttachShader(shader_program, vertex_shader.name);
    glAttachShader(shader_program, fragment_shader.name);
    glLinkProgram(shader_program);

    // delete unnecessary shaders
    glDeleteShader(vertex_shader.name);
    glDeleteShader(fragment_shader.name);
    free(dummy_1);
    free(dummy_2);
    dummy_1 = NULL; // we are freeing dummys again at the end of the program so setting to NULL is just good practice.
    dummy_2 = NULL;
    
    // Generate VBO AND VAO and EBO
    glGenBuffers(1, &VBO);
    if(VBO == GL_INVALID_VALUE){
        fprintf(stderr, "ERROR: VBO failed to be generated\n");
        glDeleteProgram(shader_program);
        goto error;
    }
    glGenVertexArrays(1, &VAO);
    if(VAO == GL_INVALID_VALUE){
        fprintf(stderr, "ERROR: VAO failed to be generated\n");
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shader_program);
        goto error;
    }
    glGenBuffers(1, &EBO);
    if(EBO == GL_INVALID_VALUE){
        fprintf(stderr, "ERROR: EBO failed to be generated\n");
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(shader_program);
        goto error;
    }
    // Bind vertex array, vertex buffer and element buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    // buffer data to array buffer and element buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Taking Position input from object into buffer 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbinding VAO and VBO (not necessary but to be safe)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Opening and setting YUV/RGB file and parameters
    if(select == 1 || select == 2){
        data = rohan_read_YUV_image(my_image, img_width, img_height, select);

        if(data){
            if(select == 1) // selecting YUV420 image
                rohan_texture_YUV_420(data, y_plane, u_plane, v_plane, &texture_1, &texture_2, &texture_3, img_width, img_height);
            if(select == 2)
                rohan_texture_YUV_422(data, y_plane, u_plane, v_plane, &texture_1, &texture_2, &texture_3, img_width, img_height);
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
        fprintf(stderr, "ERROR: Failed to load texture(s)\n");
        goto preerror;
    }

    // Give fragment shader their respective textures
    glUseProgram(shader_program);
    if(select == 1 || select == 2){
        glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
        glUniform1i(glGetUniformLocation(shader_program, "texture2"), 1);
        glUniform1i(glGetUniformLocation(shader_program, "texture3"), 2);
    }
    else if(select == 3)
        glUniform1i(glGetUniformLocation(shader_program, "texture_1"), 0);
    

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
        if(select == 1 || select == 2){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture_2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, texture_3);
        }
        else if(select == 3){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_1);
        }

        // Set shader program
        glUseProgram(shader_program);
        
        // Render 2 triangles
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Print FPS onto terminal
        rohan_fps_counter(&last_time, &current_time, &num_frames);

        //Switch Front and Back Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    fprintf(stdout, "\n");

    preerror:
    // Delete buffers to free memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);
    goto end;

    error:
    fprintf(stderr, "\nOOPS! Something went wrong!\n");
    main_err = EXIT_FAILURE;

    end:   
    free(dummy_1);
    free(dummy_2);
    
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