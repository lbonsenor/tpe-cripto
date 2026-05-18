#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdint.h>
#define MAX 50
#define SET 10

static int64_t seed;

/// @brief Inicializa la `seed` a partir de un numero dado
/// @param s El numero con el que se calculará `seed`
void
setSeed(int64_t s);

/// @brief Calcula un unsigned `char` pseudoaleatorio
/// @return El caracter calculado
uint8_t
nextChar(void);

static int64_t seed = 0;

#endif