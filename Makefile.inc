# ─── Compilador y flags ───────────────────────────────────────────────────────
CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -g
LDFLAGS =

# ─── Nombre del ejecutable ────────────────────────────────────────────────────
TARGET  = visualSSS

# ─── Directorios ──────────────────────────────────────────────────────────────
SRC_DIR = src
INC_DIR = src/include
OBJ_DIR = obj
BIN_DIR = bin

# ─── Fuentes y objetos ────────────────────────────────────────────────────────
SRCS    = $(wildcard $(SRC_DIR)/*.c)          \
          $(wildcard $(SRC_DIR)/args/*.c)

# Aplana la ruta al reemplazar src/<subdir>/foo.c → obj/<subdir>/foo.o
OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))