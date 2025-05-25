#include "bmp24.h"
#include <stdlib.h>
#include <string.h>


//function that takes as parameter the width and height of the image. 
//It allocates memory for a 2D array of pixels (t_pixel) and returns a pointer to it.
t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    //allocating memory for the array of array of pixels
    t_pixel **pixels = (t_pixel **)malloc(sizeof(t_pixel *) * height);
    //check if the allocation was successful
    if (pixels == NULL) {
        printf("Not enough memory to allocate");
        return NULL;
    }
    //loop that goes through the rows
    for (int i = 0; i < height; i++) {
        //goes through the rows to allocate memory of the sub arrays
        pixels[i] = (t_pixel*)malloc(width * sizeof(t_pixel));
        //check if the allocation was successful
        if (pixels[i]==NULL) {
            printf("No enough memory to allocate");
            for (int j = 0; j < i; j++) {
                //if fail, it must free each column one by one
                free(pixels[j]);
            }
            return NULL;
        }
    }
    return pixels;
}


//function that takes as parameter a pointer to a 2D array of pixels and the height of the image.
//It frees the memory allocated for the array of pixels.
void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    //loop that goes through all the rows and frees them
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}


//function that takes as parameters the width, height, and color depth of the image.
//It allocates memory for a t_bmp24 element returning a pointer to it.
t_bmp24 * bmp24_allocate (int width, int height, int colorDepth) {
    //allocate memory for a t_bmp24 element
    t_bmp24 *image = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (image == NULL) {
        printf("Memory allocation failed");
        return NULL;
    }

    //giving the elements the parameters given
    image->colorDepth = colorDepth;
    image->data = bmp24_allocateDataPixels(width , height);
    image->height = height;
    image->width = width;

    //if failed, free the data used
    if (image->data == NULL) {
        printf("Couldn't alocate data");
        free(image);
        return NULL;
    }
    return image;
}


//function that takes as parameter a pointer to a t_bmp24 element.
//It frees the memory allocated for the t_bmp24 element and its data pixels.
void bmp24_free (t_bmp24 * img) {
    //free the datas of the t_bmp24 image given
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}


//function that reads raw data from a file at a specified position.
//It takes as parameters the position in the file, a pointer to a buffer where the data will be stored,
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    //set the pointer to the specified position given
    fseek(file, position, SEEK_SET);
    //read n items from the file to stock them in the buffer
    fread(buffer, size, n, file);
}


//function that writes raw data to a file at a specified position.
//It takes as parameters the position in the file, a pointer to a buffer containing the data to be written,
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    //set the pointer to the specified position given
    fseek(file, position, SEEK_SET);
    //write n items from the buffer to the file
    fwrite(buffer, size, n, file);
}


//function that takes as parameters a pointer to a t_bmp24 element, the x and y coordinates of the pixel, and a file pointer.
//It reads the pixel value at the specified coordinates from the file and stores it in the t_bmp24 element's data array.
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    long offset = image->header.offset + ((image->height - 1 - y) * image->width + x) * 3;
    fseek(file, offset, SEEK_SET);
    unsigned char buffer[3];
    fread(buffer, 1, 3, file);
    image->data[y][x].blue = buffer[0];
    image->data[y][x].green = buffer[1];
    image->data[y][x].red = buffer[2];
}

//function that takes as parameters a pointer to a t_bmp24 element and a file pointer.
//It reads the pixel data from the file and stores it in the t_bmp24 element's data array.
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            bmp24_readPixelValue(image, x, y, file);
        }
    }
}

//function that takes as parameters a pointer to a t_bmp24 element, the x and y coordinates of the pixel, and a file pointer.
//It writes the pixel value at the specified coordinates to the file.
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    long offset = image->header.offset + ((image->height - 1 - y) * image->width + x) * 3;
    fseek(file, offset, SEEK_SET);
    unsigned char buf[3];
    buf[0] = image->data[y][x].blue;
    buf[1] = image->data[y][x].green;
    buf[2] = image->data[y][x].red;
    fwrite(buf, 1, 3, file);
}

//function that takes as parameters a pointer to a t_bmp24 element and a file pointer.
//It writes the pixel data from the t_bmp24 element's data array to the file.
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}

//function that takes as parameter a filename.
//It opens the file, reads the BMP headers, verifies that the image is 24 bits deep, and returns a pointer to a t_bmp24 element containing the image data.
t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error while opening the file, the file does not exist!\n");
        return NULL;
    }

    //temporary
    t_bmp_header header;
    t_bmp_info header_info;

    //read the headers from the file first
    file_rawRead(0, &header, sizeof(t_bmp_header), 1, file);
    file_rawRead(sizeof(t_bmp_header), &header_info, sizeof(t_bmp_info), 1, file);

    
    int width = header_info.width;
    int height = header_info.height;
    int colorDepth = header_info.bits;

    //verify that this is a 24-bit color image
    if (colorDepth != DEFAULT_DEPTH) {
        printf("The image is not 24 bits deep\n");
        fclose(file);
        return NULL;
    }

    //allocate memory for the image
    t_bmp24 *image = bmp24_allocate(width, height, colorDepth);

    //assign header fields
    image->header = header;
    image->header_info = header_info;

    //read the pixel data
    bmp24_readPixelData(image, file);

    printf("Image loaded successfully!\n\n");
    fclose(file);
    return image;
}



//function that  
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");

    //check for file opening errors
    if (file == NULL) {
        printf("Error while opening the file (doesn't exist).\n");
        return;
    }

    //write header and the image data
    file_rawWrite(0, &(img->header), sizeof(t_bmp_header), 1, file);
    file_rawWrite(sizeof(t_bmp_header), &(img->header_info), sizeof(t_bmp_info), 1, file);

    //write the pixel data
    bmp24_writePixelData(img, file);

    //close the file
    fclose(file);
}


//function to apply a negative filter (simple loop and just remplace)
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; ++y) {
        for (int x = 0; x < img->width; ++x) {
            img->data[y][x].red   = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue  = 255 - img->data[y][x].blue;
        }
    }
}


//>Function to apply the grayscale filter (simple loop and just calculate the new pixels)
void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; ++y) {
        for (int x = 0; x < img->width; ++x) {
            //average computation
            unsigned char avg = (img->data[y][x].red +
                                 img->data[y][x].green +
                                 img->data[y][x].blue) / 3;
            img->data[y][x].red   = avg;
            img->data[y][x].green = avg;
            img->data[y][x].blue  = avg;
        }
    }
}


//function that apply the brightness filter
void bmp24_brightness(t_bmp24 *img, int value) {
    int y, x;
    for (y = 0; y < img->height; y++) {
        for (x = 0; x < img->width; x++) {
            int r = img->data[y][x].red + value;
            int g = img->data[y][x].green + value;
            int b = img->data[y][x].blue + value;

            //for red
            if (r > 255)
                img->data[y][x].red = 255;
            else if (r < 0)
                img->data[y][x].red = 0;
            else
                img->data[y][x].red = r;

            //for green
            if (g > 255)
                img->data[y][x].green = 255;
            else if (g < 0)
                img->data[y][x].green = 0;
            else
                img->data[y][x].green = g;

            //for blue
            if (b > 255)
                img->data[y][x].blue = 255;
            else if (b < 0)
                img->data[y][x].blue = 0;
            else
                img->data[y][x].blue = b;
        }
    }
}


//function to apply the convolution filter
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    //usefull variables
    int k, l, half = kernelSize/2;
    float sum_r = 0, sum_g = 0, sum_b = 0;

    //loop to go over the kernel
    for (k = -half; k <= half; k++) {
        for (l = -half; l <= half; l++) {
            int px = x + l;
            int py = y + k;

            //to be sure that there is no error while behind out of the image
            if (px < 0) 
                px = 0;
            if (py < 0) 
                py = 0;
            if (px >= img->width) 
                 px = img->width - 1;
            if (py >= img->height) 
                py = img->height - 1;
            
            //multiply the pixel by the kernel value
            sum_r += img->data[py][px].red * kernel[k + half][l + half];
            sum_g += img->data[py][px].green * kernel[k + half][l + half];
            sum_b += img->data[py][px].blue * kernel[k + half][l + half];
        }
    }

    t_pixel result;

    //apply the calculation of the sum to the pixel r,g,b (check if it 0 > and 255 <)
    if (sum_r < 0)
        result.red = 0;
    else if (sum_r > 255)
        result.red = 255;
    else
        result.red = (unsigned char)(sum_r + 0.5);

    if (sum_g < 0)
        result.green = 0;
    else if (sum_g > 255)
        result.green = 255;
    else
        result.green = (unsigned char)(sum_g + 0.5);

    if (sum_b < 0)
        result.blue = 0;
    else if (sum_b > 255)
        result.blue = 255;
    else
        result.blue = (unsigned char)(sum_b + 0.5);


    //return the result (obvioulsy)
    return result;
}

//to create the kernel dinamicly
float** createKernell(float values[3][3]) {
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j];
        }
    }
    return kernel;
}

void freeKernell(float **kernel) {
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

//value of the kernels
float boxBlurKernel[3][3] = {
    {1.0/9, 1.0/9, 1.0/9},
    {1.0/9, 1.0/9, 1.0/9},
    {1.0/9, 1.0/9, 1.0/9}
};
float gaussianBlurKernel[3][3] = {
    {1.0/16, 2.0/16, 1.0/16},
    {2.0/16, 4.0/16, 2.0/16},
    {1.0/16, 2.0/16, 1.0/16}
};    
float valueOutline[3][3] = {
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
};
float valueEmboss[3][3] = {
    {-2, -1, 0},
    {-1,  1, 1},
    {0,  1, 2}
};

//to easy filter applyed
//function boxblur
void bmp24_boxBlur(t_bmp24 *img) {
    
    float **kernel = createKernell(boxBlurKernel);
    t_pixel **resultData = bmp24_allocateDataPixels(img->width, img->height);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            resultData[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }

    bmp24_freeDataPixels(img->data, img->height);
    img->data = resultData;

    freeKernell(kernel);
}

//function Gaussian blur
void bmp24_gaussianBlur(t_bmp24 *img) {
    
    float **kernel = createKernell(gaussianBlurKernel);
    t_pixel **resultData = bmp24_allocateDataPixels(img->width, img->height);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            resultData[y][x] = bmp24_convolution(img, x, y, kernel, 3);
        }
    }


    bmp24_freeDataPixels(img->data, img->height);
    img->data = resultData;

    freeKernell(kernel);
}

//function outline
void bmp24_outline(t_bmp24 *img) {

    float **outlineKernel = createKernell(valueOutline);
    t_pixel **resultData = bmp24_allocateDataPixels(img->width, img->height);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            resultData[y][x] = bmp24_convolution(img, x, y, outlineKernel, 3);
        }
    }

    bmp24_freeDataPixels(img->data, img->height);
    img->data = resultData;

    freeKernell(outlineKernel);
}

//function emboss
void bmp24_emboss(t_bmp24 *img) {
    float **embossKernel = createKernell(valueEmboss);
    t_pixel **resultData = bmp24_allocateDataPixels(img->width, img->height);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            resultData[y][x] = bmp24_convolution(img, x, y, embossKernel, 3);
        }
    }

    bmp24_freeDataPixels(img->data, img->height);
    img->data = resultData;

    freeKernell(embossKernel);
}
