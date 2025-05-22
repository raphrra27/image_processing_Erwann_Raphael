#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

//function that load the image
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

//function save image
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


//to free the memory of the momery allocated
void bmp8_free(t_bmp8 * img){
    free(img->data);
    free(img);
}

//print the info of the image
void bmp8_printInfo(t_bmp8 * img){
    printf("Image Info\n    Width: %u\n    Height: %u\n    Color depth: %u\n    Datasize: %d\n      ", img->width, img->height, img->colorDepth, img->dataSize);
}


//negative function
void bmp8_negative(t_bmp8 * img){
    for (int i = 0; i < img->height; i++){
        for (int j = 0; j < img->width; j++){
            img->data[i * img->width + j] = 255- img->data[i * img->width + j];
        }
    }
}

//brghtness function
void bmp8_brightness(t_bmp8 * img, int value){
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int index = i * img->width + j;

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

//threshold function
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int index = i * img->width + j;

            if (img->data[index] >= threshold) {
                img->data[index] = 255;
            } else {
                img->data[index] = 0;
            }
        }
    }
}


//function that apply filter for the 8
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
            //now that it's computed, we can apply the new value to the pixel
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

//we need to fin our pointer to pointer (one for free and one for dinamicly allocate memory)
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

//free kernel for 
void freeKernel(float **kernel) {
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

//function to apply the kernel (create and then apply it)
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