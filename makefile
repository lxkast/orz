CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

SRC_DIR = src
OBJ_DIR = obj

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = orz

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
