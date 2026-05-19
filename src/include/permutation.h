#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdint.h>
#define MAX 50
#define SET 10

static int64_t seed;

/// @brief Initializes the seed based on a given number
/// @param s
void
setSeed(int64_t s);

/// @brief Calculates a pseudorandom character
/// @return The calculated character
uint8_t
nextChar(void);

static int64_t seed = 0;

#endif