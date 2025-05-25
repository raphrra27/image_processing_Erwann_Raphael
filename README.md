##########################
#  Image Processing in C #
##########################

TI202I - Algorithmic and Data Structures 1
C Programming Project (2024-2025)
Proposed by Rado Rakotonarivo, Efrei Paris


Project Description: 
-------------------

This project is dedicated to image processing using the C programming language and is divided into three parts. 
The project supports both 8-bit grayscale and 24-bit color BMP images. For grayscale images, the code provides functionality for applying filters via 
convolution kernels, which allows for effects such as blur, sharpen, emboss, and outline. For color images, reading and writing functionality is 
supported for 24-bit BMP images. Additionally, the project implements histogram equalization for color images, which requires converting between RGB 
and YUV color spaces, equalizing the luminance channel, and converting back. The code is organized for modularity and maintainability and is tracked using 
a Git repository. The sample images lena_gray.bmp (grayscale) and lena_color.bmp (color) are provided for testing.

Compilation Instructions and Execution Instructions:
---------------------------------------------------

1. If you are on Clion to compile the project, simply use the make command in your terminal, assuming a Makefile is present in the project directory.
2. If you want ot do it manually:
   1. write "gcc main.c bmp8.c bmp24.c -o execution" for hte compilation
   2. then "./execution" for the executino of the prog
  

Known Bugs & Issues:
-------------------

1. Sometimes when loading the image there is an infinite loop but we didnt found the issue, sorry for that.

Credits:
--------

Developed by:
Student 1: RIERA Raphael (raphael.riera@efrei.net)
Student 2: BRIOT Erwann (erwann.briot@efrei.net)
