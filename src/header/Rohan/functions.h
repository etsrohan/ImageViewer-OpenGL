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
char* rohan_read_shader_source_code(ROHAN_IN char const* const filename);
uint8_t* rohan_read_YUV_image(ROHAN_IN char const* const filename, ROHAN_IN int const width, ROHAN_IN int const height, ROHAN_IN int const select);
void rohan_texture_YUV_420(ROHAN_REF uint8_t* const data, ROHAN_OUT uint8_t* y_plane, ROHAN_OUT uint8_t* u_plane, ROHAN_OUT uint8_t* v_plane, ROHAN_OUT unsigned int *texture_1, ROHAN_OUT unsigned int *texture_2, ROHAN_OUT unsigned int *texture_3, ROHAN_IN int const img_width, ROHAN_IN int const img_height);
void rohan_texture_YUV_422(ROHAN_REF uint8_t* const data, ROHAN_OUT uint8_t* y_plane, ROHAN_OUT uint8_t* u_plane, ROHAN_OUT uint8_t* v_plane, ROHAN_OUT unsigned int *texture_1, ROHAN_OUT unsigned int *texture_2, ROHAN_OUT unsigned int *texture_3, ROHAN_IN int const img_width, ROHAN_IN int const img_height);
void rohan_texture_RGB(ROHAN_REF uint8_t* const data, ROHAN_OUT unsigned int *texture_1, ROHAN_IN int const img_width, ROHAN_IN int const img_height);
void rohan_texture_helper(ROHAN_IN uint8_t const* const plane, ROHAN_OUT unsigned int* texture, ROHAN_IN int const img_width, ROHAN_IN int const scale_width, ROHAN_IN int const img_height, ROHAN_IN int const scale_height, ROHAN_IN int const number_channels);
void rohan_fps_counter(ROHAN_REF double* last_time, ROHAN_OUT double* current_time, ROHAN_REF double* num_frames);
void rohan_texture_YUV_optimized(ROHAN_REF uint8_t* const data, ROHAN_OUT uint8_t* y_plane, ROHAN_OUT uint8_t* u_plane, ROHAN_OUT uint8_t* v_plane, ROHAN_OUT unsigned int *texture_1, ROHAN_IN int const img_width, ROHAN_IN int const img_height, ROHAN_IN int const select);
void rohan_YUV_to_RGB_converter(ROHAN_OUT uint8_t* yuv, ROHAN_IN int const img_width, ROHAN_IN int const img_height);
//============================================================================================================================================================================

#endif //ROHAN_MY_FUNCTIONS_H