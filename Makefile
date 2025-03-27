default:
	clang++ -std=c++17 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -Llib -Iinclude -lraylib src/main.cpp -o main
