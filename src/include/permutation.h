#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdint.h>
#define MAX 50
#define SET 10

extern int64_t seed;

/// @brief Initializes the seed based on a given number
/// @param s
void
setSeed(int64_t s);

/// @brief Calculates a pseudorandom character
/// @return The calculated character
uint8_t
nextChar(void);

/// @brief Generates a width x height table with pseudorandom characters
/// @param table
/// @param width
/// @param height
void
generate_table(uint8_t* table, int32_t width, int32_t height);

#endif
