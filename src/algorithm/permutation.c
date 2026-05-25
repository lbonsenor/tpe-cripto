#include "permutation.h"

int64_t seed = 0;

void
setSeed(int64_t s) {
    seed = (s ^ 0x5DEECE66DL) & ((1LL << 48) - 1);
}

uint8_t
nextChar(void) {
    seed = (seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
    return (uint8_t)(seed >> 40);
}

void
generate_table(uint8_t* table, int32_t width, int32_t height) {
    for (int i = 0; i < width * height; i++) {
        table[i] = nextChar();
    }
}
