include Makefile.inc

.PHONY: all clean fclean re

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@
	@echo "Ejecutable generado: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
	@echo "  CC  $<"

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	@echo "Objetos eliminados."

fclean: clean
	rm -rf $(BIN_DIR)
	@echo "Ejecutable eliminado."

re: fclean all