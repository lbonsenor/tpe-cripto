#include <dirent.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "algorithm.h"
#include "args.h"
#include "bmp.h"

void
test_path(Args* args);

char
is_bmp(const char* file);

static int
ensure_output_dir(const char* dir);

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
    if (args.mode == MODE_DISTRIBUTE || args.mode == MODE_RECOVER) {
        if (args.n == 0)
            printf("n       : (usar todas las del directorio)\n");
        else
            printf("n       : %d\n", args.n);
    }
    printf("Dir     : %s\n", args.dir);
    if (args.mode == MODE_DISTRIBUTE) {
        printf("Output  : %s\n", args.output);
    }

    switch (args.mode) {
        case MODE_DISTRIBUTE: {
            BMPImage* img = read_bmp(args.secret_path);

            char s_paths[MAX_CARRIERS][256];
            verify_directory(&args, img, s_paths);

            if (ensure_output_dir(args.output) != 0) {
                print_error(ERR_CANNOT_OPEN_DIR);
                free_bmp(img);
                return ERR_CANNOT_OPEN_DIR;
            }

            distribute(img, args.k, args.n, s_paths, args.output);
            break;
        }
        case MODE_RECOVER:{
            printf("Recovering secret image...\n");
            char s_paths[MAX_CARRIERS][256];
            if (verify_recovery_directory(&args, args.k, s_paths) < 0) {
                return ERR_INVALID_BMP;
            }
            recover(args.secret_path, args.k, args.n, s_paths);
            break;
        }

        default:
            break;
    }

    /* TODO: llamar a distribute() o recover() según args.mode */
    // test_path(&args);

    return ERR_OK;
}

static int
ensure_output_dir(const char* dir) {
    struct stat st;

    if (dir == NULL || dir[0] == '\0') {
        return -1;
    }

    if (stat(dir, &st) == 0) {
        return S_ISDIR(st.st_mode) ? 0 : -1;
    }

    if (mkdir(dir, 0755) == 0) {
        return 0;
    }

    return -1;
}

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
    printf("Top-left pixel value: %d\n", top_left);

    char s_paths[MAX_CARRIERS][256];
    printf("Shadow images count: %d", verify_directory(args, img, s_paths));

    free_bmp(img);
}

char
is_bmp(const char* file) {
    size_t len = strlen(file);
    return (len > 4 && strcasecmp(file + len - 4, ".bmp") == 0);
    return 0;
}
