.DEFAULT_GOAL := start
	
start: main clean
	
main: glad
	@rm -f main
	clang++ --std=gnu++23 main.cpp glad.o -I ../includes -L ../libs -l glfw3 -framework OpenGL -framework Cocoa -framework IOKit -o main
	./main

glad:
	clang glad.c -I ../includes -c -o glad.o

clean:
	@echo "Cleaning..."
	@rm -f glad.o