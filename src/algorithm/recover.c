#include "algorithm.h"
#include "bmp.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
TODO Step 0. Recover seed/indexes
TODO Step 1. Set the current processing section j to 1. 
TODO Step 2. Take a non-processed pixel of position j from each of the r shadow images. 
TODO Step 3. Use these r pixels fj(1), fj(2), fj(3),..., fj(r) and Lagrange’s interpolation to solve the coefficients a0, a1, a2 ,..., ar-1 in Eq. 5. They are exactly the r pixel values of the j-th section in Q. 
TODO Step 4. Increase j by 1. 
TODO Step 5. Repeat Steps 2 to 4 until all pixels of the shadow images are processed. 
TODO Step 6. Apply the XOR operation to the predefined random image R and Q to get the secret image O’.
*/

// CHOCLO GIGANTE. Para la persona que lo lea y quiera entender. 
    // En cada imagen shadow, cada pixel tiene en el byte 6,7 un seed. 8y9 un index. 
    // Los shadows se evaluan de a sectiones de k pixels.
    // entonces vos tenes almacenado para cada shadow, f_j(index).
    // Ej. para el shadow de index 0, f_1(0), f_2(0), f_3(0) ... f_n(0)
    // Para el shadow de index 1, f_1(1), f_2(1), f_3(1) ... f_n(1)
    // Para ir recuperando el secreto, agarras los valores obetnidos f_1(0), f_1(1), f_1(2) ... f_1(n-1) 
    // y haces la interpolacion de Lagrange para obtener a0, a1, a2, a3 ... a_k-1.

    // Esos coeficientes me sirven para recuperar la section j del secreto.
    // Luego haces lo mismo para la section j+1, j+2 ... hasta recuperar todo el secreto.


int
find_shadows_and_seed(char s_paths[MAX_CARRIERS][256], int n, BMPImage* s_imgs[n], int64_t* seed);

static int
bits_for_k(int k);

static uint8_t
extract_shadow_byte(const BMPImage* shadow, int k, int section_num);

static int
lagrange_interpolation(uint8_t shadow_values[], int k,uint8_t out_coeffs[]);

int
recover(char* out_path, int k, int n, char s_paths[MAX_CARRIERS][256]) {

    BMPImage* s_imgs[n];
    int64_t seed=-1;
    
    for (int i = 0; i < n; i++) {
        s_imgs[i] = NULL;
    }

    /*
        Step 0. 
        Recover seed/indexes
    */
    find_shadows_and_seed(s_paths, n, s_imgs, &seed);
    /*
        Step 1. 
        Set the current processing section j to 1.
    */
    int32_t section_num = 0;
    int32_t img_size = s_imgs[0]->width * s_imgs[0]->height;
    BMPImage * to_return = (BMPImage*)malloc(sizeof(BMPImage));
    to_return->width = s_imgs[0]->width;
    to_return->height = s_imgs[0]->height;
    to_return->data = (uint8_t*)malloc(sizeof(uint8_t) * img_size);

    for (; section_num < img_size / k; section_num++) {

        /*
            Step 2.
            agarras los valores obtuvidos f_1(0), f_1(1), f_1(2) ... f_1(n-1) 
        */
        uint8_t shadow_values[k];
        for (int i = 0; i < k; i++) {
            shadow_values[i] = extract_shadow_byte(s_imgs[i], k, section_num);
        }
        /*
            Step 3.
            Lagrange Interpolation para obtener a0, a1, a2, a3 ... a_k-1.
        */
        uint8_t out_coeffs[k];
        lagrange_interpolation(shadow_values,k,out_coeffs);
        for(int i = 0; i < k; i++){
            to_return->data[section_num * k + i] = out_coeffs[i];
        }
    }

    //TODO
    /*
        Step 6. 
        Apply the XOR operation to the predefined random image R and Q to get the secret image O’.
    */

    //TODO
    // Save the recovered image at out_path

    // free all allocated memory

    for (int i = 0; i < n; i++) {
        free_bmp(s_imgs[i]);
    }

    return 0;
}

int
find_shadows_and_seed(char s_paths[MAX_CARRIERS][256], int n, BMPImage* s_imgs[n], int64_t* seed) {

    for (int i = 0; i < n; i++) {
        BMPImage* shadow = read_bmp(s_paths[i]);
        if (!shadow) {
            return -1;
        }

        uint16_t index;
        uint16_t seed_to_compare;
        if (read_shadow_metadata(s_paths[i], &seed_to_compare, &index) < 0) {
            free_bmp(shadow);
            return -1;
        }
        
        if(*seed == -1) {
            *seed = seed_to_compare;
        } 
        
        if (*seed != seed_to_compare || index >= (uint16_t)n || s_imgs[index] != NULL) {
            free_bmp(shadow);
            return -1;
        }
        s_imgs[index] = shadow;
    }

    return 0;
}

static int
bits_for_k(int k) {
    int bits = 8 / k;
    if (8 % k != 0)
        bits++;
    if (bits <= 1)
        return 1;
    if (bits <= 2)
        return 2;
    return 4;
}

static uint8_t
extract_shadow_byte(const BMPImage* shadow, int k, int section_num) {
    int bits_per_pixel  = bits_for_k(k);
    int pixels_per_byte = 8 / bits_per_pixel;
    uint8_t mask        = (uint8_t)((1u << bits_per_pixel) - 1u);
    uint32_t pixel_start = (uint32_t)section_num * (uint32_t)pixels_per_byte;
    uint8_t value = 0;

    for (int bit_group = 0; bit_group < pixels_per_byte; bit_group++) {
        uint8_t bits = (uint8_t)(shadow->data[pixel_start + bit_group] & mask);
        int shift = 8 - bits_per_pixel * (bit_group + 1);
        value |= (uint8_t)(bits << shift);
    }

    return value;
}

static int
lagrange_interpolation(uint8_t shadow_values[], int k, uint8_t out_coeffs[]) {
    
    
    return 0; 
}
