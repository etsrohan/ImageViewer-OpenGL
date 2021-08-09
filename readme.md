This is the readme file for the Orqa FPV miniproject. This documents contains my progress. Ideas and thought process.

The project problem statement:
 Take in RAW image data from a file and display it using OpenGL then optimize the program for my GPU.

Project Started Tuesday 03/08/2021:
Initial googling and setting up libraries and header files (glfw and glad). Keep getting function not defined error.
Started reading 'getting started' page on glfw website: https://www.glfw.org/docs/latest/quick.html

Wednesday 04/08/2021:
Took some help from discord buddies and got libraries setup.
Started studying about graphics pipeline and all the various function available in glfw. (Initializing, opening window, Vertex Buffer, Vertex Array, Element Bufffer.)
I don't know how I'm going to approach the project given.
This resource is great! : https://learnopengl.com/Introduction
Things to do: Make a square, find out how to load image onto square. Optimize
Things I don't know: How to load image, format of raw image, what is the plan

Friday 06/08/2021:
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

Saturday 07/08/2021:
I made a Makefile using the GNU Make build system to make the compiling process easier by just typing make into cmd.
Made it so that the shaders (vertex and fragment) were their own files (.shader) in the the shaders folder. These were giving a bit of a problem:
I was using fopen to open the file location, seeking forward to see how many chars the file had (set as length), seeking back to set file pointer to beginning, mallocing the required amount of memory to be set to a char* pointer (buffer) and then reading the file using fread (reading only the length number of elements). But it was giving me garbage values attached to the end of the buffer. I tried mallocing length + 1 memory and adding '\0' to the last char and still giving me garbage values.
This took way too long to solve. The solution I came up with is that I noticed that all the chars I typed into the files came up. So, I JUST END MY FILES WITH A '//end' and once I close the file_pointer (fclose(file_pointer)), I just loop backwards from length until it finds the '//' at the end and replace the first '/' with '\0'.
And voila! The shaders loaded in alright from then on.
Then I started reading up on textures to display the image.
Things to do: display an image in RGB, Optimize for GPU.
Things I dont know: do we still need to know how to load in RAW image files? How to apply texture to my square.

Sunday 08/08/2021:
After reading up on textures and how they are loaded up to the GL_TEXTURE_2D buffer and mipmapped and all that. I displayed a .jpg file onto the square. I used an image loader library called stbi_image.h (Added it to my Makefile).
Yay! I can display an image on my square! And I can make 2 textures and overlap them too! I made the square as big as the window that was opened and boom image loader is in action.
Things to do: Make a shader to load YUV images as well as RGB images (maybe separate shaders depending on which type of image is used), Optimize for GPU.
Things I dont know: RAW image file loading needed? What is YUV image format. How Im going to optimize for GPU.

Monday 09/08/2021:
Part 1:
I noticed that some of the images were not being loaded in / displayed properly (this happened mainly with jpg file format), so I added the line "glPixelStorei(GL_UNPACK_ALIGNMENT, 1);" before loading the image into the texture buffer. Also as a fun experiment I made the first texture values vary with time using a sine wave uniform sent to the fragment shader.