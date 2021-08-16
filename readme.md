**HOW TO USE THE CODE:**

image_display.c and image_display_NV_optimized contain the main functions.

In both image_display.c and image_display_NV_optimized.c scroll down to the section containing IMAGE INPUT DATA.
Input the name of the image as a string with the extension (.jpg, .bmp, .yuv, .uyvy).
Input the width and height of the image (required for YUV files, not so much for other formats)
Enter the integer value in 'select' (1 = YUV420, 2 = YUV422 (uyvy), 3 = RGB)
Make the file using the Makefile and it should be ready.

This is the readme file for the Orqa FPV miniproject. This documents contains my progress. Ideas and thought process.

**The project problem statement:**

 Take in RAW image data from a file and display it using OpenGL then optimize the program for my GPU.

**Project Started Tuesday 03/08/2021:**

Initial googling and setting up libraries and header files (glfw and glad). Keep getting function not defined error.
Started reading 'getting started' page on glfw website: https://www.glfw.org/docs/latest/quick.html

**Wednesday 04/08/2021:**

Took some help from discord buddies and got libraries setup.
Started studying about graphics pipeline and all the various function available in glfw. (Initializing, opening window, Vertex Buffer, Vertex Array, Element Bufffer.)
I don't know how I'm going to approach the project given.
This resource is great! : https://learnopengl.com/Introduction
Things to do: Make a square, find out how to load image onto square. Optimize
Things I don't know: How to load image, format of raw image, what is the plan

**Friday 06/08/2021:**

Finally made a triangle and then a SQUARE!
Finally got an idea of how to solve the project problem.
I know that the FRAGMENT SHADER fills in the color pixels for the screen so if I can manipulate what pixels it is filling then I might be able to display an image in a window.
Steps for success:

    1) Make Square as big as screen itself
    
    2) Load in the image data and perhaps process it or use library to convert it to RBG values.
    
    3) Looking forward there is a texture chapter on the website I am using. Take a look at that.
    
    4) Apply a texture to the square or apply a shader to it (not mutually exclusive) and display an image.
    
    5) Profit.
    
Things to do: Use GNU Make to make things easier, Ask about RAW image format. Apply image onto square.

Things I dont know: how to load a RAW image (format of RAW image), how to apply texture to square. How to optimize for GPU.

**Saturday 07/08/2021:**

I made a Makefile using the GNU Make build system to make the compiling process easier by just typing make into cmd.
Made it so that the shaders (vertex and fragment) were their own files (.shader) in the the shaders folder. These were giving a bit of a problem:
I was using fopen to open the file location, seeking forward to see how many chars the file had (set as length), seeking back to set file pointer to beginning, mallocing the required amount of memory to be set to a char* pointer (buffer) and then reading the file using fread (reading only the length number of elements). But it was giving me garbage values attached to the end of the buffer. I tried mallocing length + 1 memory and adding '\0' to the last char and still giving me garbage values.
This took way too long to solve. The solution I came up with is that I noticed that all the chars I typed into the files came up. So, I JUST END MY FILES WITH A '//end' and once I close the file_pointer (fclose(file_pointer)), I just loop backwards from length until it finds the '//' at the end and replace the first '/' with '\0'.
And voila! The shaders loaded in alright from then on.
Then I started reading up on textures to display the image.
Things to do: display an image in RGB, Optimize for GPU.
Things I dont know: do we still need to know how to load in RAW image files? How to apply texture to my square.

**Sunday 08/08/2021:**

After reading up on textures and how they are loaded up to the GL_TEXTURE_2D buffer and mipmapped and all that. I displayed a .jpg file onto the square. I used an image loader library called stbi_image.h (Added it to my Makefile).
Yay! I can display an image on my square! And I can make 2 textures and overlap them too! I made the square as big as the window that was opened and boom image loader is in action.
Things to do: Make a shader to load YUV images as well as RGB images (maybe separate shaders depending on which type of image is used), Optimize for GPU.
Things I dont know: RAW image file loading needed? What is YUV image format. How Im going to optimize for GPU.

**Monday 09/08/2021:**

Part 1:
I noticed that some of the images were not being loaded in / displayed properly (this happened mainly with jpg file format), so I added the line "glPixelStorei(GL_UNPACK_ALIGNMENT, 1);" before loading the image into the texture buffer. Also as a fun experiment I made the first texture values vary with time using a sine wave uniform sent to the fragment shader.

Part 2:
I spent the rest of the day coming up with an algorithm to unpack the YUV420 image file. And made the function too. My thinking was at this point that:

1) I would load 3 arrays with the respective  Y, U and V values.
2) Then I will assign the values of U and V to their respective posistions using this algorithm in a for loop looping through all the values of U and V
    if(i % (width / 2) == 0) loop_index++;
    chrominanceU[i * 2 + img_width * loop_index] = img_dummy[i];
    chrominanceU[i * 2 + img_width * loop_index + 1] = img_dummy[i];
    chrominanceU[i * 2 + img_width * loop_index + img_width] = img_dummy[i];
    chrominanceU[i * 2 + img_width * loop_index + img_width + 1] = img_dummy[i];

**Tuesday 10/08/2021:**

I shared my plan to display the YUV420 image on discord with Orqa and I was given a much needed adjustment on my thinking direction: Instead of putting the u and v values into their respective positions, since Y,U and V values are planar, let the fragment shader take care of upscaling it and attaching it to the respective texture coordinates.
At 11 pm  I was able to load and display the image onto a window in OpenGL. It's upside down and has a green bar at the top but otherwise its THERE! These are easy fixes.
Things to do: fix the yuv420 image and start working on the yuv422 image.
Things I do not know: How to unpack the yuv422 image.

**Wednesday 11/08/2021:**

At 10 am I was able to get rid of the green bar at the bottom. I couldn't find anything all night last night but then this morning I read that images are binary files... then it clicked, I was using fopen(filename, "r"), so I changed it to fopen(filename, "rb") 
At around 5 pm, after coming back from work and starting to work on the YUV422 unpacking, the program now displays both yuv420 format and yuv422 (uyvy) format. The the user has to input the width/height, name and mode (select) of the image he/she wants to display. These are all at the top of the body of main.
Up next is running OpenGL on my graphics card and optimizing the program for it.

**Thursday 12/08/2021:**

Today I formatted my code according to the pdf document 'orqa_c_guide', added a bunch of error handeling and file restructuring. Also I started reading about the OpenGL extensions that I would need to get started on optimizing my program. Came a long way, a long way ahead, this will be fun.

**Friday 13/08/2021:**

I added a simple block of code to display the FPS and Milliseconds per Frame onto the terminal to get an idea of the performance of the program. I have been reading some websites on how to optimize the code for my GPU but its not clicking yet, lets give it some more time and continue reading.

**Saturday 14/08/2021:**

I was searching the internet for ways to optimize my program. I looked at bottleneck tests, GPU architecture and everything I could think of, after reading the extensions page, https://www.khronos.org/opengl/wiki/OpenGL_Extension. I was looking for what extensions I could add to improve the performance of my simple image loader. Then it hit me. The glad loader page had a bunch of extensions at the bottom which I had ignored. So I started skimming through that list of extensions until I came across this... GL_NV_DRAW_TEXTURES, https://www.khronos.org/registry/OpenGL/extensions/NV/NV_draw_texture.txt, and this has a function called glDrawTextureNV. This amazing piece of code just outright rendered 70% of my efforts useless. So, instead of drawing 2 triangles to make a rectangle, specifying texture coordinates, setting up all the buffers (VAO, VBO, EBO), sending all that data to shaders and letting them do the work, this program skips all that and directly draws on the the coordinates you give it. No need to set anything up, no need to use load, compile and use shaders. The documentation said it gives better performance (although I couldn't see it, might be driver capped) and really lowers power consumption. After that it was just about generating and binding textures and pass it on to glDrawTextureNV and boom, IMAGE, ON, SCREEN. So, I did just that, made entire functions that, unpack YUV422 an YUV420, and then convert YUV format data into RGB format (with a few hiccups at first), and it was done. I separated the original image_display .c file into image_display.c (NVIDIA optimized) and image_display_all.c (code that runs on all platforms, unoptimized), changed the Makefile too. Tomorrow I am gonna combine the two into one .c code and ask if I have to do more. This mini project might be coming to an end.

**Sunday 15/08/21:**

So I decided to rename the 2 main .c files. The unoptimized for NVIDIA GPU is called image_display.c (makes image_display.exe) and the one optimized for NVIDIA cards is image_display_NV_optimized.c (makes image_display_NV_optimized.exe). The optimized version is giving me a lot better performance as compared to the normal version, (285 - 300 -> 300 - 315 fps). I tried running the optimized version on the internal integrated graphics and it crashed as expected. Maybe I will combine the two of them into one mega program and beyond this I don't think I can optimize it further, because the optimized version gets rid of the fragment/vertex shaders, buffer objects, shader program, everything (Only requires textures).
