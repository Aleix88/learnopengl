CC = clang
CXX = clang++

INCLUDES = -I $(HOME)/include
LIBRARIES = -L $(HOME)/libs

CXXFLAGS = -g -O0 --std=gnu++23 $(INCLUDES) $(LIBRARIES)

LDFLAGS = -lglfw3 -lglad

TARGET = main

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) main.cpp $(LDFLAGS) -o $@

clean:
	@echo "Cleaning..."
	@rm -f *.o $(TARGET)

.PHONY: clean all