#include "bmp24.h"
#include <stdlib.h>
#include <string.h>




t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    t_pixel **pixels = (t_pixel **)malloc(sizeof(t_pixel *) * height);
    if (pixels == NULL) {
        printf("Not enough memory to allocate");
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel*)malloc(width * sizeof(t_pixel));
        if (pixels[i]==NULL) {
            printf("No enough memory to allocate");
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            return NULL;
        }
    }
    return pixels;
}




void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    if (pixels == NULL) {
        printf("Already freed");
        return;
    }
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}



t_bmp24 * bmp24_allocate (int width, int height, int colorDepth) {
    t_bmp24 *image = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (image == NULL) {
        printf("Memory allocation failed");
        return NULL;
    }

    image->colorDepth = colorDepth;
    image->data = bmp24_allocateDataPixels(width , height);
    image->height = height;
    image->width = width;

    if (image->data == NULL) {
        printf("Couldn't alocate data");
        free(image);
        return NULL;
    }
    return image;
}


void bmp24_free (t_bmp24 * img) {
    if (img == NULL) {
        printf("Already freed");
        return;
    }
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}






void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file) {

}






