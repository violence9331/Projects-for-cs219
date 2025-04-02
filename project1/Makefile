# CC = gcc
# CFLAGS = -g -Iinclude
# SRC_DIR = src
# OBJ_DIR = obj
# BIN_DIR = bin

# SOURCES = $(wildcard $(SRC_DIR)/*.c)
# OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
# TARGET = $(BIN_DIR)/calculator

# $(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# # 生成目标文件的规则
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# all: $(TARGET)

# $(TARGET): $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $@ $^ -lm

# clean:
# 	rm -f $(OBJ_DIR)/*.o $(TARGET)

# # 新增的运行目标
# run: $(TARGET)
# 	./$(TARGET)

# .PHONY: all clean run 
# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -g

# 目标可执行文件名
TARGET = calculator

# 源文件
SRC = calculator.c

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lm

# 清理生成的文件
clean:
	rm -f $(TARGET)

# 重新编译
rebuild: clean all

# 伪目标
.PHONY: all clean rebuild