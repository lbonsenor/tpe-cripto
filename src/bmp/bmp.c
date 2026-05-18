#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

BMPImage*
read_bmp(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file)
        return NULL;

    BMPFileHeader file_header;
    BMPInfoHeader info_header;

    fread(&file_header, sizeof(BMPFileHeader), 1, file);
    fread(&info_header, sizeof(BMPInfoHeader), 1, file);

    // Validación de formato 8-bit
    if (file_header.type != 0x4D42 || info_header.bits_per_pixel != 8) {
        fprintf(stderr, "Error: No es un tipo de BMP válido\n");
        fclose(file);
        return NULL;
    }

    int32_t width  = info_header.width;
    int32_t height = info_header.height;

    int is_bottom_up = (height > 0);
    if (height < 0)
        height = -height;

    BMPImage* image = malloc(sizeof(BMPImage));
    image->width    = width;
    image->height   = height;
    image->data     = malloc(width * height * sizeof(uint8_t));

    int padding = (4 - (width) % 4) % 4;

    /*
        Leer bien el valor que indica en qué offset empieza la matriz de píxeles, 
        ya que puede comenzar inmediatamente después de los 54 bytes del encabezado, 
        o bien empezar más adelante.
    */
    fseek(file, file_header.offset, SEEK_SET);

    for (int i = 0; i < height; i++) {
        int target_row   = is_bottom_up ? (height - 1 - i) : i;
        uint8_t* row_ptr = &image->data[target_row * width];

        fread(row_ptr, sizeof(uint8_t), width, file);

        if (padding > 0) {
            fseek(file, padding, SEEK_CUR);
        }
    }

    fclose(file);
    return image;
}

void
free_bmp(BMPImage* image) {
    if (image) {
        if (image->data)
            free(image->data);
        free(image);
    }
}