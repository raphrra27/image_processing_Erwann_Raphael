#include <stdlib.h>
#include <string.h>
#include "bmp8.h"

//To load the image
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

//To save the image in the same folder or repository
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


//filter negative function
void bmp8_negative(t_bmp8 * img){
    for (int i = 0; i < img->height; i++){
        for (int j = 0; j < img->width; j++){
            img->data[i+j*img->width] = 255- img->data[i+j*img->width];
        }
    }
}

//filter brghtness function
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

//filter threshold function
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

//For the filter with the kernel
//faire un parcours de l'image sauf les bords applique le programme et mettre le pixel dans une copie de l'image pour eviter les problemes, les pixels de bords en change pas !!! 
//donc faire un nouvelle image avec free etc a la fin 
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) {
    int width = img->width;
    int height = img->height;

}

