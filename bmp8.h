#include <stdio.h>
typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
    } t_bmp8;


//Definition of the kernel used in the apply filter
//For the blur
float boxBlur[3][3] = {
    {1.0/9, 1.0/9, 1.0/9},
    {1.0/9, 1.0/9, 1.0/9},
    {1.0/9, 1.0/9, 1.0/9}
};

//For the gaussian
float gaussianBlur[3][3] = {
    {1.0/16, 2.0/16, 1.0/16},
    {2.0/16, 4.0/16, 2.0/16},
    {1.0/16, 2.0/16, 1.0/16}
};

//For the outline
float outline[3][3] = {
    {-1, -1, -1},
    {-1, 8, -1},
    {-1, -1, -1}
};

//For the edge detection
float edgeDetection[3][3] = {
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
};

//For the sharpen
float sharpen[3][3] = {
    { 0, -1,  0},
    {-1,  5, -1},
    { 0, -1,  0}
};


    
//To save an image
t_bmp8 *bmp8_loadImage(const char *filename);
//To save an image
void bmp8_saveImage(const char * filename, t_bmp8 * img);
//to free the memory of the momery allocated
void bmp8_free(t_bmp8 * img);
//print the info of the image
void bmp8_printInfo(t_bmp8 * img);
//negative function
void bmp8_negative(t_bmp8 * img);
//brghtness function
void bmp8_brightness(t_bmp8 * img, int value);
//threshold function
void bmp8_threshold(t_bmp8 *img, int threshold);
//to apply the ends of the filters 
void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize);

    