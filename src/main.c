#include <stdio.h>
#include <stdlib.h>

#include "args.h"

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

    return ERR_OK;
}