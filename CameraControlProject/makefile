.DEFAULT_GOAL := r

d: main
	lldb main
	clean

r: main
	./main
	clean
	
main:
	@rm -f main
	clang++ -g -O0 --std=gnu++23 main.cpp -I ~/include -L ~/libs -l glfw3 -l glad -o main

clean:
	@echo "Cleaning..."

cleanall: clean
	@rm -rf main main.dSYM
