.DEFAULT_GOAL := r

d: main
	lldb main
	clean

r: main
	./main
	clean
	
main: glad
	@rm -f main
	clang++ -g -O0 --std=gnu++23 main.cpp glad.o -I ../includes -L ../libs -l glfw3 -framework OpenGL -framework Cocoa -framework IOKit -mmacosx-version-min=15.6 -o main

glad:
	clang -g -O0 glad.c -I ../includes -c -o glad.o

clean:
	@echo "Cleaning..."
	@rm -rf glad.o stb_image.o

cleanall: clean
	@rm -rf main main.dSYM
