/////////////////HEADER////////////////
//Authors: Riera Raphael Briot Erwann//
//Description: file .c that define all the functions for gray image, thus bmp8 file//
//Role: its the part 1 and 3of the project so for gray images//


//INCLUDE + VARIABLES

#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

//Define all the kernel (with the values) needed for the filter (more clean for the main)
float valuesBoxBlur[3][3] = {
    {1/9.0f, 1/9.0f, 1/9.0f},
    {1/9.0f, 1/9.0f, 1/9.0f},
    {1/9.0f, 1/9.0f, 1/9.0f}
};

float valuesGaussianBlur[3][3] = {
    {1/16.0f, 2/16.0f, 1/16.0f},
    {2/16.0f, 4/16.0f, 2/16.0f},
    {1/16.0f, 2/16.0f, 1/16.0f}
};

float valuesOutline[3][3] = {
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
};

float valuesEmboss[3][3] = {
    {-2, -1, 0},
    {-1,  1, 1},
    {0,  1, 2}
};



//FUNCTIONS

//function that takes as parameter a filename.
//It loads a BMP image from the specified file and returns a pointer to a t_bmp8 structure containing the image data.
t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *image = fopen(filename, "rb");

    //error when the file is not existing
    if (image == NULL) {
        printf("Error while opening the file, the file does not exist!\n");
        return NULL;
    }

    //extract image properties from the BMP header
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);

    // Extract image properties from the header
    unsigned int width = *(unsigned int*)&header[18];
    unsigned int height = *(unsigned int*)&header[22];
    unsigned short colorDepth = *(unsigned short*)&header[28];
    unsigned int dataSize = *(unsigned int*)&header[34];

    //error when the file is not 8 bits deep
    if (colorDepth != 8) {
        printf("Error while opening the file, the file is not 8 bits deep!\n");
        fclose(image);
        return NULL;
    }

    //allocate memory for the BMP image
    t_bmp8 *bmpImage = (t_bmp8 *)malloc(sizeof(t_bmp8));


    // Copy header and color table
    memcpy(bmpImage->header, header, 54);
    fread(bmpImage->colorTable, sizeof(unsigned char), 1024, image);


    //initialize the image proprietes
    bmpImage->width = width;
    bmpImage->height = height;
    bmpImage->colorDepth = colorDepth;
    bmpImage->dataSize = dataSize;

    //allocate memory
    bmpImage->data = (unsigned char *)malloc(dataSize);

    //move to the pixel data location in the file and read the data
    fseek(image, *(unsigned int*)&header[10], SEEK_SET);
    fread(bmpImage->data, sizeof(unsigned char), dataSize, image);

    //close + print
    fclose(image);
    printf("Image loaded successfully!\n\n");
    return bmpImage;
}

//function that takes as parameters a filename and a pointer to a t_bmp8 structure.
//It saves the BMP image data from the t_bmp8 structure to the specified file.
void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    //open the file for writing in binary mode
    FILE *file = fopen(filename, "wb");
    if (!file) {

        //if file cannot be opened, print an error message and return
        printf("Error during the opening of the file, (gdb work)");
        return;
    }

    //BMP header
    size_t header = fwrite(img->header, sizeof(unsigned char), 54, file);
    if (header != 54) {
        printf("Error occured while writing the header");
        fclose(file);
        return;
    }

    //BMP color table
    size_t ColorTable = fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    if (ColorTable != 1024) {
        printf("Error during the writing of color table");
        fclose(file);
        return;
    }

    //bMP pixel data
    size_t dataWritten = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    if (dataWritten != img->dataSize) {
        printf("Error during the writing of pixel data");
        fclose(file);
        return;
    }

    //close the file
    fclose(file);
}


//function that takes as parameter an allocated t_bmp8 structure.
//It frees the memory allocated for the t_bmp8 structure and its data.
void bmp8_free(t_bmp8 * img){
    free(img->data);
    free(img);
}

//function that prints the information of a t_bmp8 structure.
void bmp8_printInfo(t_bmp8 * img){
    printf("Image Info\n    Width: %u\n    Height: %u\n    Color depth: %u\n    Datasize: %d\n      ", img->width, img->height, img->colorDepth, img->dataSize);
}


//function that takes as parameter a pointer to a t_bmp8 structure.
//It applies a grayscale filter to the image by converting each pixel to its grayscale equivalent.
void bmp8_negative(t_bmp8 * img){
    for (int i = 0; i < img->height; i++){
        for (int j = 0; j < img->width; j++){
            img->data[i * img->width + j] = 255- img->data[i * img->width + j];
        }
    }
}

//function that takes as parameter a pointer to a t_bmp8 structure and a value.
//It applies a brightness filter to the image by adjusting the brightness of each pixel by the specified value.
void bmp8_brightness(t_bmp8 * img, int value){
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int index = i * img->width + j;

            //conditions if the value goes up to 255 or under 0
            if ((img->data[index] + value) > 255) {
                img->data[index] = 255;
            } else if ((img->data[index] + value) < 0) {
                img->data[index] = 0;
            } else {
                img->data[index] = img->data[index] + value;
            }
        }
    }
}

//function that takes as parameter a pointer to a t_bmp8 structure and an integer threshold.
//It applies a threshold filter to the image by setting pixels above the threshold to 255 and below to 0.
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int index = i * img->width + j;

            //conditions if the value goes up to 255 or under 0
            if (img->data[index] >= threshold) {
                img->data[index] = 255;
            } else {
                img->data[index] = 0;
            }
        }
    }
}


//function that  takes as parameters a pointer to a t_bmp8 structure, a kernel, and the size of the kernel.
//It applies a filter to the image by convolving each pixel with the kernel.
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    //going through the whole image
    for (int y = 1; y < img->height-1; y++) {
        for (int x = 1; x < img->width-1; x++) {

            float newvalue = 0.0;

            //by going through the kernel matrix
            for (int i = -kernelSize/2; i <= kernelSize/2; i++ ) {
                for (int j = -kernelSize/2; j <= kernelSize/2; j++) {

                    int neighborX = x + j;
                    int neighborY = y + i;
                    int index = neighborY * img->width + neighborX;
                    newvalue += img->data[index] * kernel[i+ kernelSize/2][j+ kernelSize/2];
                }
            }

            //now that its computed, we can apply the new value to the pixel
            int currentIndex = y * img->width + x;
            if (newvalue > 255) {
                newvalue = 255;
            } else if (newvalue < 0) {
                newvalue = 0;
            }
            img->data[currentIndex] = newvalue;
        }
    }
}


//function that takes as parameter a 2D array of float values.
//It creates a kernel from the 2D array and returns a pointer to the kernel. it returns a pointer to the kernel
float** createKernel(float values[3][3]) {
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = values[i][j];
        }
    }
    return kernel;
}

//function that takes as parameter a pointer to a 2D array of floats representing a kernel.
//It frees the memory allocated for the kernel.
void freeKernel(float **kernel) {
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}


//functions that takes as parameter a pointer to a t_bmp8 structure.
//It applies various filters to the image, such as box blur, gaussian blur, outline, and emboss.


void bmp8_boxblur(t_bmp8 * img){
    float **boxBlurKernel = createKernel(valuesBoxBlur);
    bmp8_applyFilter(img, boxBlurKernel, 3);
    freeKernel(boxBlurKernel);
}

void bmp8_gaussian(t_bmp8 * img){
    float **gaussianBlurKernel = createKernel(valuesGaussianBlur);
    bmp8_applyFilter(img, gaussianBlurKernel, 3);
    freeKernel(gaussianBlurKernel);
}

void bmp8_outline(t_bmp8 * img){
    float **outlineKernel = createKernel(valuesOutline);
    bmp8_applyFilter(img, outlineKernel, 3);
    freeKernel(outlineKernel);
}

void bmp8_emboss(t_bmp8 * img){
    float **embossKernel = createKernel(valuesEmboss);
    bmp8_applyFilter(img, embossKernel, 3);
    freeKernel(embossKernel);
}


//function that takes as parameter a pointer to a t_bmp8 structure.
//It computes the histogram of the image and returns a pointer to an array containing the histogram values. 
unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    unsigned int *histogram = (unsigned int*)calloc(256, sizeof(unsigned int));

    //check if there is an historgram pointer
    if (!histogram){
        return NULL;
    } 
    
    //going through the imgae
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int currentindex = y * img->width + x;
            unsigned char pixelValue = img->data[currentindex];
            histogram[pixelValue]++;
        }
    }

    return histogram;
}


//function that takes as parameter a pointer to an array containing the histogram values.
//It computes the cumulative distribution function (CDF) from the histogram and returns a pointer to an array containing the CDF values.
unsigned int * bmp8_computeCDF(unsigned int * hist) {
    unsigned int *cdf = (unsigned int*)calloc(256, sizeof(unsigned int));

    //check if there is cdf
    if (!cdf){
        return NULL;
    } 

    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i-1] + hist[i];
    }
    return cdf;
}



//function that takes as parameters a pointer to a t_bmp8 structure and a pointer to an array containing the CDF values.
//It applies histogram equalization to the image using the CDF values.
void bmp8_equalize(t_bmp8 *img, unsigned int *cdf) {
    unsigned int cdf_min = 0, totalPixels = img->width * img->height;

    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    unsigned char hist_eq[256];
    for (int i = 0; i < 256; i++) {
        hist_eq[i] = (unsigned char)(
            ((float)(cdf[i] - cdf_min) / (totalPixels - cdf_min)) * 255.0f + 0.5f
        );
    }

    for (int i = 0; i < totalPixels; i++) {
        img->data[i] = hist_eq[(unsigned char)img->data[i]];
    }
}
