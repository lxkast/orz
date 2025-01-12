CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source and object files
SRCS = $(SRC_DIR)/orz.c $(SRC_DIR)/utils.c $(SRC_DIR)/terminal.c
OBJS = $(OBJ_DIR)/orz.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/terminal.o

# Target executable
TARGET = orz

# Default rule
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Ensure the object directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
