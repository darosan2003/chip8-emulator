# Variable for the compiler
CC = gcc
# Variables for compiler flags
CFLAGS = -Wall -c -g3
# Variables for linking flags
CLINKERS = -lSDL2 -o
# Variable for object directory
OBJ_DIR = /obj/
# Variable to store the targets
TARGET = chip8
# Variable to store the objects
OBJ = main.o chip8.o

.PHONY: all
all: dir $(TARGET)

$(TARGET): $(OBJ)
	@echo "[+] Generating the executable..."
	@cd .$(OBJ_DIR); $(CC) $^ $(CLINKERS) $@; mv $@ ../$@
	@echo "[+] Done!"

%.o: %.c
	@echo "[+] Generating the .o file..."
	$(CC) $(CFLAGS) $<
	@mv *.o .$(OBJ_DIR)

.PHONY: clean
clean:
	@echo "[+] Removing .$(OBJ_DIR) directory and the executables..."
	@if [ -d .$(OBJ_DIR) ] && [ -f $(TARGET) ]; then rm -rf .$(OBJ_DIR) $(TARGET); echo "[+] Done!"; else echo "[-] .$(OBJ_DIR) or $(TARGET) don't exist"; fi

.PHONY: dir
dir:
	@echo "Creating .$(OBJ_DIR) directory..."
	@if [ ! -d .$(OBJ_DIR) ]; then mkdir .$(OBJ_DIR); echo "[+] Done!"; else echo "[-] .$(OBJ_DIR) already exists"; fi
