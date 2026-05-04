#ifndef ARGS_H
#define ARGS_H

#define MIN_K 2
#define MAX_K 10
#define MIN_N 2

#define BMP_HEADER_SIZE 54

#define BMP_SEED_BYTE_LO 6   /* LSB de la semilla de permutación      */
#define BMP_SEED_BYTE_HI 7   /* MSB de la semilla de permutación      */
#define BMP_SHADOW_BYTE_LO 8 /* LSB del número de orden de la sombra  */
#define BMP_SHADOW_BYTE_HI 9 /* MSB del número de orden de la sombra  */

#define ERR_OK 0
#define ERR_INVALID_MODE 1         /* Se especificó -d y -r a la vez, o ninguno        */
#define ERR_MISSING_SECRET 2       /* Falta el parámetro -secret                        */
#define ERR_MISSING_K 3            /* Falta el parámetro -k                             */
#define ERR_K_OUT_OF_RANGE 4       /* k < MIN_K o k > MAX_K                             */
#define ERR_N_OUT_OF_RANGE 5       /* n < MIN_N o n < k                                 */
#define ERR_N_WITH_RECOVER 6       /* -n fue usado junto con -r (solo válido con -d)    */
#define ERR_INVALID_ARGUMENT 7     /* Argumento desconocido o mal formado               */
#define ERR_FILE_NOT_FOUND 8       /* No se pudo abrir el archivo indicado              */
#define ERR_DIR_NOT_FOUND 9        /* No se pudo abrir el directorio indicado           */
#define ERR_NOT_ENOUGH_IMAGES 10   /* No hay suficientes portadoras en el directorio    */
#define ERR_IMAGE_SIZE_MISMATCH 11 /* Las portadoras no tienen el mismo tamaño          */
#define ERR_INVALID_BMP 12         /* El archivo BMP no cumple los requisitos pedidos   */
#define ERR_MEMORY 13              /* Fallo de reserva de memoria dinámica              */

typedef enum {
    MODE_UNSET      = -1,
    MODE_DISTRIBUTE = 0, /* -d : ocultar imagen secreta en portadoras */
    MODE_RECOVER    = 1  /* -r : recuperar imagen secreta             */
} Mode;

typedef struct {
    Mode mode;         /* Modo de operación (distribuir / recuperar) */
    char* secret_path; /* Ruta al archivo BMP secreto                */
    int k;             /* Mínimo de sombras para recuperar           */
    int n;             /* Total de sombras a generar (solo con -d)   */
    char* dir;         /* Directorio de portadoras (default: ".")    */
} Args;

/// @brief Parsea argc/argv y rellena la estructura Args.
/// @param argc
/// @param argv
/// @param out
/// @return Devuelve ERR_OK en caso de éxito o un código de error en caso contrario. En caso de error imprime un mensaje descriptivo y la sintaxis correcta.
int
parse_args(int argc, char* argv[], Args* out);

/// @brief Imprime la sintaxis de uso del programa.
/// @param program_name
void
print_usage(const char* program_name);

/// @brief Imprime el mensaje correspondiente al código de error dado.
/// @param error_code
void
print_error(int error_code);

#endif