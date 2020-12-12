CC = gcc
CXX = g++

LIB = ./lib

GLAD = $(LIB)/glad
GLAD_SRC = $(GLAD)/src/glad.c
GLAD_INC = $(GLAD)/include

GLM = $(LIB)/glm

INC = -I$(GLAD_INC) -I$(LIB) -I$(GLM)
LDLIBS = -lX11 -lglfw3 -lGL -lpthread -ldl
CFLAGS = -Wall -I$(GLAD_INC)
CXXFLAGS = -std=c++11 -Wall $(INC)

TARGET = main

SRC = ./src/main.cpp $(wildcard ./src/**/*.cpp)

objects = $(SRC:.cpp=.o) glad.o

all: $(TARGET)

$(TARGET): $(objects)
	$(CXX) -o $@ $^ $(LDLIBS)

glad.o: $(GLAD_SRC)
	$(CC) $(CFLAGS) -c -o glad.o $(GLAD_SRC)

clean:
	rm $(TARGET) $(objects)
