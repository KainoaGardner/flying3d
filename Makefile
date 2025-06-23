CC := gcc
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -I/usr/include/freetype2
CFLAGS := -Wall -Iinclude

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib

EXEC := $(BIN_DIR)/run

SRCS := $(SRC_DIR)/main.cpp \
				$(SRC_DIR)/geometry.cpp \
				$(SRC_DIR)/shader.cpp \
				$(SRC_DIR)/player.cpp \
				$(SRC_DIR)/utils.cpp \
				$(SRC_DIR)/key.cpp \
				$(SRC_DIR)/bullet.cpp \
				$(SRC_DIR)/particle.cpp \
				$(SRC_DIR)/ships.cpp \
				$(SRC_DIR)/display.cpp \
				$(SRC_DIR)/config.cpp \
				$(SRC_DIR)/text.cpp \
				$(SRC_DIR)/boss.cpp \
				$(SRC_DIR)/textures.cpp \
				$(LIB_DIR)/glad.c \
				$(LIB_DIR)/stb_image.cpp

OBJS := $(OBJ_DIR)/main.o \
				$(OBJ_DIR)/geometry.o \
				$(OBJ_DIR)/shader.o \
				$(OBJ_DIR)/player.o \
				$(OBJ_DIR)/utils.o \
				$(OBJ_DIR)/key.o \
				$(OBJ_DIR)/text.o \
				$(OBJ_DIR)/bullet.o \
				$(OBJ_DIR)/config.o \
				$(OBJ_DIR)/display.o \
				$(OBJ_DIR)/particle.o \
				$(OBJ_DIR)/boss.o \
				$(OBJ_DIR)/ships.o \
				$(OBJ_DIR)/textures.o \
				$(OBJ_DIR)/glad.o \
				$(OBJ_DIR)/stb_image.o

LIBS := -lGL -lglfw -ldl -lX11 -lpthread -lfreetype


all: $(EXEC)

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

.PHONY: all clean
