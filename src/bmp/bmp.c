#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

static int
read_u16_at(FILE* file, long offset, uint16_t* out) {
    if (fseek(file, offset, SEEK_SET) != 0)
        return -1;
    if (fread(out, sizeof(uint16_t), 1, file) != 1)
        return -1;
    return 0;
}

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
    if (file_header.type != BMP_TYPE_BM || info_header.bits_per_pixel != 8) {
        fprintf(stderr, "Error: No es un tipo de BMP válido\n");
        fclose(file);
        return NULL;
    }

    int32_t width  = info_header.width;
    int32_t height = info_header.height;

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
        
        Los píxeles se almacenan en memoria en el mismo orden físico del archivo
        (fila 0 en memoria = primera fila en el archivo). Para BMP bottom-up esto
        significa que data[0] es la fila inferior de la imagen visualmente, pero
        el algoritmo de distribución/recuperación opera sobre bytes en orden físico,
        por lo que debe ser consistente entre distribute y recover.
    */
    fseek(file, file_header.offset, SEEK_SET);

    for (int i = 0; i < height; i++) {
        uint8_t* row_ptr = &image->data[i * width];

        fread(row_ptr, sizeof(uint8_t), width, file);

        if (padding > 0) {
            fseek(file, padding, SEEK_CUR);
        }
    }

    fclose(file);
    return image;
}

int
write_bmp(BMPImage* image, const char* filename) {
    if (!image || !image->data || image->width <= 0 || image->height <= 0) {
        fprintf(stderr, "Error: Imagen inválida provista para guardar.\n");
        return -1;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: No se pudo crear el archivo '%s'\n", filename);
        return -1;
    }

    int padding          = (4 - (image->width) % 4) % 4;
    uint32_t matrix_size = (image->width + padding) * image->height;

    uint32_t headers_and_palette_size = 54 + 1024;
    BMPFileHeader file_header;
    file_header.type      = BMP_TYPE_BM;
    file_header.size      = headers_and_palette_size + matrix_size;
    file_header.reserved1 = 0; // RESERVADO PARA LA SEED E INDICE
    file_header.reserved2 = 0;
    file_header.offset    = headers_and_palette_size;

    BMPInfoHeader info_header;
    info_header.size             = 40;
    info_header.width            = image->width;
    info_header.height           = image->height;
    info_header.planes           = 1;
    info_header.bits_per_pixel   = 8;
    info_header.compression      = 0;
    info_header.image_size       = matrix_size;
    info_header.x_pixels_per_m   = 2835;
    info_header.y_pixels_per_m   = 2835;
    info_header.colors_used      = 256;
    info_header.colors_important = 0;

    fwrite(&file_header, sizeof(BMPFileHeader), 1, file);
    fwrite(&info_header, sizeof(BMPInfoHeader), 1, file);

    for (int i = 0; i < 256; i++) {
        uint8_t palette_entry[4] = { i, i, i, 0 };
        fwrite(palette_entry, sizeof(uint8_t), 4, file);
    }

    uint8_t padding_bytes[3] = { 0, 0, 0 };
    for (int i = 0; i < image->height; i++) {
        /*
         * Los píxeles están en memoria en orden físico (fila 0 = primera fila del archivo).
         * Escribimos en el mismo orden para mantener consistencia con el algoritmo.
         */
        uint8_t* row_ptr = &image->data[i * image->width];

        fwrite(row_ptr, sizeof(uint8_t), image->width, file);

        if (padding > 0) {
            fwrite(padding_bytes, sizeof(uint8_t), padding, file);
        }
    }

    fclose(file);
    return 0;
}

int
write_shadow_metadata(const char* path, uint16_t seed, uint16_t shadow_id) {
    FILE* file = fopen(path, "r+b");
    if (!file)
        return -1;

    // Navigate to Byte 6 (Reserved 1)
    fseek(file, 6, SEEK_SET);
    // Write Seed (Takes up 2 bytes)
    fwrite(&seed, sizeof(uint16_t), 1, file);

    // Navigate to Byte 8 (Reserved 2)
    fseek(file, 8, SEEK_SET);
    // Write Shadow Order Identification Number (Takes up 2 bytes)
    fwrite(&shadow_id, sizeof(uint16_t), 1, file);

    fclose(file);

    return 0;
}

int
read_shadow_metadata(const char* path, uint16_t* seed_out, uint16_t* shadow_id_out) {
    if (!path || !seed_out || !shadow_id_out)
        return -1;

    FILE* file = fopen(path, "rb");
    if (!file)
        return -1;

    // Seed is stored in bytes 6-7 (reserved1)
    // Shadow index is stored in bytes 8-9 (reserved2)
    if (read_u16_at(file, 6, seed_out) != 0 || read_u16_at(file, 8, shadow_id_out) != 0) {
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

void
free_bmp(BMPImage* image) {
    if (image) {
        if (image->data)
            free(image->data);
        free(image);
    }
}