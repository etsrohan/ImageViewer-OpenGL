#include "header/glad.h" //gladLoadGL
#define GLF_INCLUDE_NONE
#include "header/glfw3.h" //glfwFunctions

#include "header/stb_image.h" //stbi_load, stbi_image_free
#include "header/Rohan/functions.h"

#include <stdio.h> //printf
#include <stdlib.h> //fopen, fread, fseek
#include <string.h> //memcpy

// Function Definitions
char* rohan_read_shader_source_code(ROHAN_IN char const* const filename){
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
        if(fclose(file_pointer)){
            free(buffer);
            buffer = NULL;
            return buffer;
        }
            
        // Go through file backwards and where ever it finds '//' replace first '/' with '\0'
        for(i = length - 1; i > 0; i--){
            if(buffer[i] == '/' && buffer[i + 1] == '/'){
                buffer[i] = '\0';
                break;
            }
        }
    }

    return buffer;
}

uint8_t* rohan_read_YUV_image(ROHAN_IN char const* const filename, ROHAN_IN int const width, ROHAN_IN int const height, ROHAN_IN int const select){
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
        if(fclose(file_pointer)){
            data = NULL;
        }
    }
    else{
        printf("ERROR: FILE FAILED TO LOAD\n");
    }
    return data;
}

void rohan_texture_YUV_420(ROHAN_REF uint8_t* const data, ROHAN_OUT uint8_t* y_plane, ROHAN_OUT uint8_t* u_plane, ROHAN_OUT uint8_t* v_plane, ROHAN_OUT unsigned int *texture_1, ROHAN_OUT unsigned int *texture_2, ROHAN_OUT unsigned int *texture_3, ROHAN_IN int const img_width, ROHAN_IN int const img_height){
    /* This function sets up the textures for the YUV420 (yuv) image file by setting up the y,u and v_plane arrays and binding them to 3 individual single channel textures.*/
    // Setting up y/u/v_plane to pass to shader
    y_plane = data;
    u_plane = data + img_width * img_height;
    v_plane = u_plane + (img_width * img_height / 4);

    // Send the plane values to shader as separate shaders
        // Y Values
    rohan_texture_helper(y_plane, texture_1, img_width, 1, img_height, 1, 1);
        // U Values
    rohan_texture_helper(u_plane, texture_2, img_width, 2, img_height, 2, 1);
        // V Values
    rohan_texture_helper(v_plane, texture_3, img_width, 2, img_height, 2, 1);
}

void rohan_texture_YUV_422(ROHAN_REF uint8_t* const data, ROHAN_OUT uint8_t* y_plane, ROHAN_OUT uint8_t* u_plane, ROHAN_OUT uint8_t* v_plane, ROHAN_OUT unsigned int *texture_1, ROHAN_OUT unsigned int *texture_2, ROHAN_OUT unsigned int *texture_3, ROHAN_IN int img_width, ROHAN_IN int img_height){
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
    rohan_texture_helper(y_plane, texture_1, img_width, 1, img_height, 1, 1);
        // U Values
    rohan_texture_helper(u_plane, texture_2, img_width, 2, img_height, 1, 1);
        // V Values
    rohan_texture_helper(v_plane, texture_3, img_width, 2, img_height, 1, 1);

    // Free calloc'd memory
    free(y_plane);
    free(u_plane);
    free(v_plane);
}

void rohan_texture_RGB(uint8_t* const data, unsigned int *texture_1, int const img_width, int const img_height){
    rohan_texture_helper(data, texture_1, img_width, 1, img_height, 1, 3);
}

void rohan_texture_helper(ROHAN_IN uint8_t const* const plane, ROHAN_OUT unsigned int* texture, ROHAN_IN int const img_width, ROHAN_IN int const scale_width, ROHAN_IN int const img_height, ROHAN_IN int const scale_height, ROHAN_IN int const number_channels){
    /*This function sets up a texture plane based on given image width and height
      It scales the image depending on the scaling factors image_dimension / scale_dimension
      If number_channels = 1: single vector of texture made, number_channels = 3: vec3 texture made*/
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(plane){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if(number_channels == 1)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, img_width / scale_width, img_height / scale_height, 0, GL_RED, GL_UNSIGNED_BYTE, plane);
        else if(number_channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, plane);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else printf("ERROR: %p values failed to load\n", plane);
}

void rohan_fps_counter(ROHAN_REF double* last_time, ROHAN_OUT double* current_time, ROHAN_REF double* num_frames){
    /* This function displays the frames per second 
    and time to draw 1 frame on the terminal*/
    *current_time = glfwGetTime();
    *num_frames += 0.001;
    if(*current_time - *last_time >= 1.0){    // If the last print statement was more than a second ago
        printf("\rFPS: %0.1f, Milliseconds Per Frame: %f", *num_frames * 1000.0, 1.0 / *num_frames);
        *num_frames = 0;
        *last_time += 1.0;
    }
}

void rohan_texture_YUV_optimized(ROHAN_REF uint8_t* const data, ROHAN_OUT uint8_t* y_plane, ROHAN_OUT uint8_t* u_plane, ROHAN_OUT uint8_t* v_plane, ROHAN_OUT unsigned int *texture_1, ROHAN_IN int const img_width, ROHAN_IN int const img_height, ROHAN_IN int const select){
    /* This function takes in raw YUV data and converts 
    it to RGB data for the YUV420 and YUV422 (uyvy) formats*/
    uint8_t *formatted_data = calloc(img_width * img_height * 3, sizeof(uint8_t)); // Final formatted data
    int loop_index;
    int i; // for loop index

    if(select == 1){
        // Unpack image data
        
        y_plane = data;
        u_plane = data + img_width * img_height;
        v_plane = u_plane + img_width * img_height / 4;

        // Arrange YUV420 (yuv) format: yyyyyy...uuuuu...vvvvv -> y,u,v,y,u,v
        /* For the algorithm I explain it in readme file
            Basic idea is that I calculate the corresponding y position 
            for u and v values and I put them in order of 3 byte y,u,v 
            so yi -> 3 * yi, so ui -> 3 * ui + 1 and vi -> 3 * vi +2*/
        for(i = 0; i < img_width * img_height; i++){ // Sorting the y values
            formatted_data[3 * i] = y_plane[i];
        }

        loop_index = -1;
        for(i = 0; i < img_width * img_height / 4; i++){ // looping through every u value
            if(i % (img_width / 2) == 0) loop_index++;
            formatted_data[3 * (i * 2 + img_width * loop_index) + 1]                  = u_plane[i]; // Sorting the u values (single u value corresponds to 4 y values)
            formatted_data[3 * (i * 2 + img_width * loop_index + 1) + 1]              = u_plane[i];
            formatted_data[3 * (i * 2 + img_width * loop_index + img_width) + 1]      = u_plane[i];
            formatted_data[3 * (i * 2 + img_width * loop_index + img_width + 1) + 1]  = u_plane[i];
        }

        loop_index = -1;
        for(i = 0; i < img_width * img_height / 4; i++){ // looping through every v value
            if(i % (img_width / 2) == 0) loop_index++;
            formatted_data[3 * (i * 2 + img_width * loop_index) + 2]                  = v_plane[i]; // Sorting the v values
            formatted_data[3 * (i * 2 + img_width * loop_index + 1) + 2]              = v_plane[i];
            formatted_data[3 * (i * 2 + img_width * loop_index + img_width) + 2]      = v_plane[i];
            formatted_data[3 * (i * 2 + img_width * loop_index + img_width + 1) + 2]  = v_plane[i];
        }

    }
    else if(select == 2){
        // Unpack image data and arrange into YUV422 (uyvy) format: u,y,v,y,u,y,v,y -> y,u,v,y,u,v
        /*Basic idea here is that I am converting 4 bytes into 6 bytes
          so u0 y0 v0 y1 -> y0 u0 v0 y1 u0 v0
              0  1  2  3 ->  1  0  2  3  0  2*/
        for(int i = 0; i < img_width * img_height / 2; i++){ // looping through every 4 bytes of image data (every 4 bytes contais 2 pixels and total number of pixels is width*height)
            formatted_data[6 * i]       = data[4 * i + 1]; 
            formatted_data[6 * i + 1]   = data[4 * i];
            formatted_data[6 * i + 2]   = data[4 * i + 2];
            formatted_data[6 * i + 3]   = data[4 * i + 3];
            formatted_data[6 * i + 4]   = data[4 * i];
            formatted_data[6 * i + 5]   = data[4 * i + 2];
        }
    }

    // Convert YUV image to RGB image
    rohan_YUV_to_RGB_converter(formatted_data, img_width, img_height);

    // Generate Texture from converted RGB image
    rohan_texture_helper(formatted_data, texture_1, img_width, 1, img_height, 1, 3);

    free(formatted_data);
}

void rohan_YUV_to_RGB_converter(ROHAN_OUT uint8_t* yuv, ROHAN_IN int const img_width, ROHAN_IN int const img_height){
    /*This function takes in image data in YUV format and converts it to RGB format
      (input) y u v y u v y u v -> r g b r g b r g b (output) */
    uint8_t red, green, blue;
    uint8_t y, u, v;

    for(int i = 0; i < img_width * img_height; i ++){ // Looping through every pixel
        y = yuv[3 * i];
        u = yuv[3 * i + 1];
        v = yuv[3 * i + 2];

        if((1.164 * (y - 16) + 1.596 * (v - 128)) <= 255 && ((1.164 * (y - 16) + 1.596 * (v - 128)) >= 0)) red = (uint8_t) (1.164 * (y - 16) + 1.596 * (v - 128));
        else if (((1.164 * (y - 16) + 1.596 * (v - 128)) > 255)) red = 255;
        else red = 0; 

        if((1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128)) <= 255 && (1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128)) >= 0) green = (uint8_t) (1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128));
        else if ((1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128)) > 255) green = 255;
        else green = 0; 

        if((1.164 * (y - 16) + 2.018 * (u - 128)) <= 255 && (1.164 * (y - 16) + 2.018 * (u - 128)) >= 0) blue = (uint8_t) (1.164 * (y - 16) + 2.018 * (u - 128));
        else if ((1.164 * (y - 16) + 2.018 * (u - 128)) > 255) blue = 255;
        else blue = 0;  

        yuv[3 * i]      = red;
        yuv[3 * i + 1]  = green;
        yuv[3 * i + 2]  = blue;
    }
}