#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "args.h"
#include "bmp.h"

int
distribute(BMPImage* img, int k, int n, char s_paths[MAX_CARRIERS][256], const char* output_dir);
int
recover(char* out_path, int k, int n, char s_paths[MAX_CARRIERS][256]);

#endif
