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

    t_bmp8 *bmp8_loadImage(const char *filename);
    //To savec an image
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
    //apply a filter using matrices
    void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize);
    //to create our kernels
    float** createKernel(float values[3][3]);
    //free ou kernel 
    void freeKernel(float **kernel);
    //box blur function
    void bmp8_boxblur(t_bmp8 * img);
    //gaussian filter
    void bmp8_gaussian(t_bmp8 * img);
    //emboss function filter
    void bmp8_emboss(t_bmp8 * img);
    //outline function filter
    void bmp8_outline(t_bmp8 * img);


    