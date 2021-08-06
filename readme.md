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
Things I dont know: how to load a RAW image (format of RAW image), how to apply texture to square. How to optimize for GPU.
