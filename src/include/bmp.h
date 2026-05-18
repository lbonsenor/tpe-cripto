#ifndef BMP_H
#define BMP_H

#include <stdint.h>

#pragma pack(push, 1)

// https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
#define BMP_TYPE_BM 0x4D42 // Windows 3.1x, 95, NT, ... etc.
#define BMP_TYPE_BA 0x4142 // OS/2 struct bitmap array
#define BMP_TYPE_CI 0x4943 // OS/2 struct color icon
#define BMP_TYPE_CP 0x5043 // OS/2 const color pointer
#define BMP_TYPE_IC 0x4349 // OS/2 struct icon
#define BMP_TYPE_PT 0x5450 // OS/2 pointer

typedef struct {
    uint16_t type; // Must be 0x4D42 ('BM')
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset; // CRITICAL: This tells us exactly where the pixels start
} BMPFileHeader;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel; // This MUST be 8 now!
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_m;
    int32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t colors_important;
} BMPInfoHeader;

#pragma pack(pop)

typedef struct {
    int32_t width;
    int32_t height;
    uint8_t* data; // 1D array of single bytes (grayscale values)
} BMPImage;

/// @brief Parses a file to the BMP image struct
/// @param filename
/// @return
BMPImage*
read_bmp(const char* filename);

/// @brief Liberates the BMPImage from memory
/// @param image
void
free_bmp(BMPImage* image);

#endif
