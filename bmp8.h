#include <stdio.h>
#include <stdint.h>

//different structures needed in the programm
typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
    } t_bmp8;

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    } t_bmp_header;

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
    } t_bmp24;

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


// Offsets for the BMP header
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34

// Magical number for BMP files
#define BMP_TYPE0x4D42 // 'BM' in hexadecimal

// Header sizes
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets

// Constant for the color depth
#define DEFAULT_DEPTH 0x18 // 24

//we need to have matrices for the kernels

    
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

