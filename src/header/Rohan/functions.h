#ifndef ROHAN_MY_FUNCTIONS_H
#define ROHAN_MY_FUNCTIONS_H

// Function argument macros
#define ROHAN_NOARGS
#define ROHAN_IN
#define ROHAN_REF
#define ROHAN_OUT

// Shader struct
typedef struct {
    unsigned int name;
    char const* src_code;
} RohanMyShader; // This struct contains essential information regarding loading a shader.

// Flag Enum
typedef enum {
    ROHAN_OK                 =  0,
    ROHAN_GLFW_INIT_ERR      =  1,
    ROHAN_WINDOW_INIT_ERR    =  1,
} RohanErrorFlag;

// I don't know how to implement this without using a global variable as the flgwSetKeyCallback function is set for a specific function pointer
static int wireframe = 1; // a boolean to keep track of wireframe mode or fill mode

//============================================================================================================================================================================
// Function Declarations

//This function takes in the file path from input and sends back the source
//code for the specified shader
char* rohan_read_shader_source_code(
    ROHAN_IN char const* const filename
);

//This function takes in a YUV format (Raw) and loads it with its 3 components, 
//luminance (Y component), chrominanceU (U component), chrominanceV (V component)
//and returns a single pointer
//
//select = 1 : YUV420 format, select = 2 : UYVY (YUV422) format
//
//Note: This function does require that the user be aware of the width and height of the image
uint8_t* rohan_read_YUV_image(
    ROHAN_IN char const* const filename,
    ROHAN_IN int const width, 
    ROHAN_IN int const height, 
    ROHAN_IN int const select
);

//This function sets up the textures for the YUV420 (yuv) image file by setting up 
//the y,u and v_plane arrays and binding them to 3 individual single channel textures.
void rohan_texture_YUV_420(
    ROHAN_REF uint8_t* const data, 
    ROHAN_OUT uint8_t* y_plane, 
    ROHAN_OUT uint8_t* u_plane, 
    ROHAN_OUT uint8_t* v_plane, 
    ROHAN_OUT unsigned int *texture_1, 
    ROHAN_OUT unsigned int *texture_2, 
    ROHAN_OUT unsigned int *texture_3, 
    ROHAN_IN int const img_width, 
    ROHAN_IN int const img_height
);

//This function sets up the textures for the YUV422 (uyvy) image file by setting up
//the y,u and v_plane arrays and binding them to 3 individual single channel textures.
void rohan_texture_YUV_422(
    ROHAN_REF uint8_t* const data, 
    ROHAN_OUT uint8_t* y_plane, 
    ROHAN_OUT uint8_t* u_plane, 
    ROHAN_OUT uint8_t* v_plane, 
    ROHAN_OUT unsigned int *texture_1, 
    ROHAN_OUT unsigned int *texture_2, 
    ROHAN_OUT unsigned int *texture_3, 
    ROHAN_IN int const img_width, 
    ROHAN_IN int const img_height
);

//This function just passes the arguments with the appropriate settings to rohan_texture_helper
//settings: width/height scale = 1 and number_channels = 3
void rohan_texture_RGB(
    ROHAN_REF uint8_t* const data, 
    ROHAN_OUT unsigned int *texture_1, 
    ROHAN_IN int const img_width, 
    ROHAN_IN int const img_height
);

//This function sets up a texture plane based on given image width and height
//
//It scales the image depending on the scaling factors image_dimension / scale_dimension
//
//If number_channels = 1: single vector of texture made, number_channels = 3: vec3 texture made
void rohan_texture_helper(
    ROHAN_IN uint8_t const* const plane, 
    ROHAN_OUT unsigned int* texture, 
    ROHAN_IN int const img_width, 
    ROHAN_IN int const scale_width, 
    ROHAN_IN int const img_height, 
    ROHAN_IN int const scale_height, 
    ROHAN_IN int const number_channels
);

//This function displays the frames per second and time to draw 1 frame on the terminal
void rohan_fps_counter(
    ROHAN_REF double* last_time, 
    ROHAN_OUT double* current_time, 
    ROHAN_REF double* num_frames
);

//This function takes in raw YUV data and converts 
//it to RGB data for the YUV420 and YUV422 (uyvy) formats
//and then converts them to textures
void rohan_texture_YUV_optimized(
    ROHAN_REF uint8_t* const data, 
    ROHAN_OUT uint8_t* y_plane, 
    ROHAN_OUT uint8_t* u_plane, 
    ROHAN_OUT uint8_t* v_plane, 
    ROHAN_OUT unsigned int *texture_1, 
    ROHAN_IN int const img_width, 
    ROHAN_IN int const img_height, 
    ROHAN_IN int const select
);

//This function takes in image data in YUV format and converts it to RGB format
//
//    (input) y u v y u v y u v -> r g b r g b r g b (output)
void rohan_YUV_to_RGB_converter(
    ROHAN_OUT uint8_t* yuv, 
    ROHAN_IN int const img_width, 
    ROHAN_IN int const img_height
);
//============================================================================================================================================================================

#endif //ROHAN_MY_FUNCTIONS_H