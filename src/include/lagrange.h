#ifndef LAGRANGE_H
#define LAGRANGE_H

#include <stdint.h>

int
lagrange_interpolation(const uint8_t shadow_values[], int k, uint8_t out_coeffs[]);

#endif
