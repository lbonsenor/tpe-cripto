#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

static const char* error_messages[] = {
    [ERR_OK]                  = "OK",
    [ERR_INVALID_MODE]        = "Debe especificar exactamente uno de los modos: -d (distribuir) o -r (recuperar).",
    [ERR_MISSING_SECRET]      = "Falta el parámetro obligatorio: -secret <imagen.bmp>.",
    [ERR_MISSING_K]           = "Falta el parámetro obligatorio: -k <número>.",
    [ERR_K_OUT_OF_RANGE]      = "El valor de k debe ser >= 2 y <= 10.",
    [ERR_N_OUT_OF_RANGE]      = "El valor de n debe ser >= 2 y >= k.",
    [ERR_N_WITH_RECOVER]      = "El parámetro -n solo puede usarse con el modo -d (distribuir).",
    [ERR_INVALID_ARGUMENT]    = "Argumento inválido o mal formado.",
    [ERR_FILE_NOT_FOUND]      = "No se pudo abrir el archivo indicado.",
    [ERR_DIR_NOT_FOUND]       = "No se pudo abrir el directorio indicado.",
    [ERR_NOT_ENOUGH_IMAGES]   = "No hay suficientes imágenes portadoras en el directorio.",
    [ERR_IMAGE_SIZE_MISMATCH] = "Las imágenes portadoras no tienen el mismo tamaño.",
    [ERR_INVALID_BMP]         = "El archivo BMP no cumple los requisitos: debe ser de 8 bits por píxel.",
    [ERR_MEMORY]              = "Error al reservar memoria dinámica.",
};

void
print_error(int error_code) {
    if (error_code > ERR_OK && error_code <= ERR_MEMORY) {
        fprintf(stderr, "ERROR: %s\n", error_messages[error_code]);
    } else {
        fprintf(stderr, "ERROR: Error desconocido (código %d).\n", error_code);
    }
}

void
print_usage(const char* program_name) {
    fprintf(stderr,
      "Uso:\n"
      "  Distribuir: %s -d -secret <imagen.bmp> -k <num> [-n <num>] [-dir <directorio>]\n"
      "  Recuperar:  %s -r -secret <imagen.bmp> -k <num> [-dir <directorio>]\n"
      "\n"
      "Parámetros obligatorios:\n"
      "  -d              Distribuir la imagen secreta en imágenes portadoras.\n"
      "  -r              Recuperar la imagen secreta desde las portadoras.\n"
      "  -secret <img>   Nombre del archivo BMP secreto (entrada en -d, salida en -r).\n"
      "  -k <num>        Mínimo de sombras para recuperar el secreto (%d <= k <= %d).\n"
      "\n"
      "Parámetros opcionales:\n"
      "  -n <num>        Total de sombras a generar (solo con -d, n >= k, n >= %d).\n"
      "  -dir <dir>      Directorio de imágenes portadoras (default: directorio actual).\n"
      "\n"
      "Ejemplos:\n"
      "  %s -d -secret clave.bmp -k 2 -n 4 -dir varias\n"
      "  %s -r -secret secreta.bmp -k 3\n",
      program_name,
      program_name,
      MIN_K,
      MAX_K,
      MIN_N,
      program_name,
      program_name);
}

int
parse_args(int argc, char* argv[], Args* out) {
    /* Valores por defecto */
    out->mode        = MODE_UNSET;
    out->secret_path = NULL;
    out->k           = 0;
    out->n           = 0; /* 0 significa "usar todas las del directorio" */
    out->dir         = ".";

    int n_set = 0;

    static struct option long_options[] = {
        { "secret", required_argument, 0, 'S' },
        { "dir", required_argument, 0, 'D' },
        { 0, 0, 0, 0 }
    };

    int opt;
    int opt_idx = 0;

    while ((opt = getopt_long_only(argc, argv, "drS:k:n:D:", long_options, &opt_idx)) != -1) {
        switch (opt) {
            case 'd':
                if (out->mode == MODE_RECOVER) {
                    print_error(ERR_INVALID_MODE);
                    print_usage(argv[0]);
                    return ERR_INVALID_MODE;
                }
                out->mode = MODE_DISTRIBUTE;
                break;

            case 'r':
                if (out->mode == MODE_DISTRIBUTE) {
                    print_error(ERR_INVALID_MODE);
                    print_usage(argv[0]);
                    return ERR_INVALID_MODE;
                }
                out->mode = MODE_RECOVER;
                break;

            case 'S':
                out->secret_path = optarg;
                break;

            case 'k': {
                int num = atoi(optarg);
                if (num < MIN_K || num > MAX_K) {
                    print_error(ERR_K_OUT_OF_RANGE);
                    print_usage(argv[0]);
                    return ERR_K_OUT_OF_RANGE;
                }
                out->k = num;
                break;
            }

            case 'n': {
                int num = atoi(optarg);
                if (num < MIN_N) {
                    print_error(ERR_N_OUT_OF_RANGE);
                    print_usage(argv[0]);
                    return ERR_N_OUT_OF_RANGE;
                }
                out->n = num;
                n_set  = 1;
                break;
            }

            case 'D':
                out->dir = optarg;
                break;

            default:
                print_error(ERR_INVALID_ARGUMENT);
                print_usage(argv[0]);
                return ERR_INVALID_ARGUMENT;
        }
    }

    if (out->mode == MODE_UNSET) {
        print_error(ERR_INVALID_MODE);
        print_usage(argv[0]);
        return ERR_INVALID_MODE;
    }

    if (out->secret_path == NULL) {
        print_error(ERR_MISSING_SECRET);
        print_usage(argv[0]);
        return ERR_MISSING_SECRET;
    }

    if (out->k == 0) {
        print_error(ERR_MISSING_K);
        print_usage(argv[0]);
        return ERR_MISSING_K;
    }

    if (n_set && out->mode == MODE_RECOVER) {
        print_error(ERR_N_WITH_RECOVER);
        print_usage(argv[0]);
        return ERR_N_WITH_RECOVER;
    }

    if (n_set && out->n < out->k) {
        print_error(ERR_N_OUT_OF_RANGE);
        print_usage(argv[0]);
        return ERR_N_OUT_OF_RANGE;
    }

    return ERR_OK;
}