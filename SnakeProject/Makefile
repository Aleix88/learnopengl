CC = clang
CXX = clang++

INCLUDES = -I $(HOME)/include -I src
LIBRARIES = -L $(HOME)/libs

CXXFLAGS = -g -O0 --std=gnu++23 $(INCLUDES) $(LIBRARIES)

LDFLAGS = -lglfw3 -lglad

OBJECTS = src/libs/stb_image.o src/SceneObjects/cube.o src/SceneObjects/snake.o src/SceneObjects/ground.o src/SceneObjects/food.o

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) src/main.cpp $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(TARGET)

.PHONY: clean all