**VisualSSS**  
Implementación en C de un esquema de **Secreto Compartido en Imágenes** con ocultamiento por **esteganografía** sobre archivos BMP de 8 bits.  
El programa permite:

- **Distribuir** una imagen secreta .bmp en n imágenes portadoras.
- **Recuperar** una imagen secreta a partir de al menos k portadoras.  
  **Requisitos**
- Linux / entorno compatible con **POSIX**
- Compilador C con soporte para C11 o posterior.
- Biblioteca estándar de C.  
  **Compilación**  
  Correr en la raiz del proyecto el comando  
  make clean all

   
**Uso**  
La sintaxis que acepta el programa es:  
./visualSSS -d -secret <imagen.bmp> -k <num> [-n <num>] [-dir <directorio>] [-o <directorio>]

   
./visualSSS -r -secret <imagen.bmp> -k <num> [-n <num>] [-dir <directorio>]

   
**Parámetros**

- -d: distribuye la imagen secreta en las portadoras.
- -r: recupera la imagen secreta desde las portadoras.
- -secret <imagen.bmp>:
  - en modo -d, indica la imagen secreta de entrada;
  - en modo -r, indica el archivo de salida donde se guardará la imagen recuperada.
- -k <num>: cantidad mínima de imágenes necesarias para reconstruir el secreto.
- -n <num>: cantidad total de sombras/portadoras a usar.
- -dir <directorio>: directorio donde están las imágenes portadoras. Si no se indica, se usa el directorio actual (.).
- -o, --output <directorio>: directorio donde se guardan las sombras (solo -d). Si no se indica, se usa el directorio actual (.).  
  **Restricciones validadas por el programa**
- k debe estar entre **2** y **10**.
- n debe ser mayor o igual que **2**.
- Si se especifica n, entonces debe cumplirse n >= k.
- La imagen secreta y las portadoras deben ser archivos .bmp.
- Para el esquema de **8 bits por píxel**, las imágenes portadoras deben ser BMP de 8 bits.
- En distribución, si no se pasa -n, el programa toma como n la cantidad de portadoras válidas encontradas en el directorio.  
  **Ejemplos**  
  Distribuir Albertssd.bmp usando k = 2, n = 4, buscando portadoras en test y guardando sombras en output:  
  ./bin/visualSSS -d -secret Albertssd.bmp -k 2 -n 4 -dir ./test -o ./output

   
Recuperar secreta.bmp usando k = 2, n = 4, desde el directorio actual, y guardando la salida en output:  
./bin/visualSSS -r  -secret secreta.bmp -k 2 -n 4 --dir output

   
**Manejo de errores**  
El programa informa errores de entrada y muestra la sintaxis correcta cuando detecta:

- modo inválido o no especificado;
- falta de -secret o -k;
- k fuera de rango;
- n inválido o menor que k;
- directorio inexistente;
- imágenes insuficientes;
- tamaños incompatibles entre portadoras;
- BMP inválido.  
  **Archivos principales**
- main.c: punto de entrada del programa.
- args.c: análisis y validación de parámetros.
- bmp.\*: lectura y escritura de imágenes BMP.
- algorithm.\*: lógica del esquema de secreto compartido.
