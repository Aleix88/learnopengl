CC = clang
CXX = clang++

INCLUDES = -I $(HOME)/include
LIBRARIES = -L $(HOME)/libs

CXXFLAGS = -g -O0 --std=gnu++23 $(INCLUDES) $(LIBRARIES)

LDFLAGS = -lglfw3 -lglad

OBJECTS = stb_image.o

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) main.cpp $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(TARGET)

.PHONY: clean all