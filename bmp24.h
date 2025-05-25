/////////////////HEADER////////////////
//Authors: Riera Raphael Briot Erwann//
//Description: file .h for the colors images, so this is the header needed fo the bmp24, thus colored images//
//Role: its the part 2 and 3 of the project so for colored image//


//INCLUDE NEEDED + STRUCT
#include <stdio.h>
#include <stdint.h>
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34
// Magical number for BMP files
#define BMP_TYPE 0x4D42 // 'BM' in hexadecimal
// Header sizes
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets
// Constant for the color depth
#define DEFAULT_DEPTH 0x18 // 24

//define struct for bmp24
#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

typedef struct {
uint8_t red;
uint8_t green;
uint8_t blue;
} t_pixel;

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;
#pragma pack(pop)


//FUNCTIONS USED IN THE BMP24

//function to read raw data from file
t_pixel ** bmp24_allocateDataPixels (int width, int height);
// function to free the data pixels
void bmp24_freeDataPixels (t_pixel ** pixels, int height);
// function to allocate memory for a t_bmp24 element
t_bmp24 * bmp24_allocate (int width, int height, int colorDepth);
// function to free the t_bmp24 element
void bmp24_free (t_bmp24 * img);
// function to read raw data from file
void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file);
// function to write raw data to file
void bmp24_readPixelData (t_bmp24 * image, FILE * file);
// function to write raw data to file
void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file);
// function to write pixel data to file
void bmp24_writePixelData (t_bmp24 * image, FILE * file);
// function to read raw data from file
t_bmp24 * bmp24_loadImage (const char * filename);
// function to save the image
void bmp24_saveImage (t_bmp24 * img, const char * filename);
// function to apply a negative filter
void bmp24_negative (t_bmp24 * img);
// function to apply the grayscale filter
void bmp24_grayscale (t_bmp24 * img);
// function to apply the brightness filter
void bmp24_brightness (t_bmp24 * img, int value);
t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize);
// function to create a kernel from a 2D array
float** createKernell(float values[3][3]);
// function to free the kernel
void freeKernell(float **kernel);
// function to apply the box blur filter
void bmp24_boxBlur(t_bmp24 *img);
// function to apply the gaussian blur filter
void bmp24_gaussianBlur(t_bmp24 *img);
// function to apply the outline filter
void bmp24_outline(t_bmp24 *img);
// function to apply the emboss filter
void bmp24_emboss(t_bmp24 *img);



