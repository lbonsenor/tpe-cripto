#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "bmp.h"

void
test_path(Args* args) {
    BMPImage* img = read_bmp(args->secret_path);
    if (!img)
        return;

    printf("Successfully loaded BMP image!\n");
    printf("Resolution: %d x %d\n", img->width, img->height);

    // Access a pixel at Coordinate (X=0, Y=0)
    // Remember: inside the data array, the values are accessed as img->data[y * width + x]
    uint8_t top_left = img->data[0 * img->width + 0];
    printf("Top-left pixel value: %d", top_left);

    free_bmp(img);
}

int
main(int argc, char* argv[]) {
    Args args;

    int err = parse_args(argc, argv, &args);
    if (err != ERR_OK) {
        return err;
    }

    /* ── Información de depuración (quitar en producción) ───────────── */
    printf("Modo    : %s\n", args.mode == MODE_DISTRIBUTE ? "DISTRIBUIR" : "RECUPERAR");
    printf("Secreto : %s\n", args.secret_path);
    printf("k       : %d\n", args.k);
    if (args.mode == MODE_DISTRIBUTE) {
        if (args.n == 0)
            printf("n       : (usar todas las del directorio)\n");
        else
            printf("n       : %d\n", args.n);
    }
    printf("Dir     : %s\n", args.dir);

    /* TODO: llamar a distribute() o recover() según args.mode */
    test_path(&args);

    return ERR_OK;
}