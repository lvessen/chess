#include <iostream>
#include "window.h"
#include <glad/glad.h>

#include "chessboard.hpp"

int main(int argc, const char * argv[]) {
	std::cout << "Hello World!" << std::endl;


	Window window(800, 600, "Hello World!");

	MakeCurrentContext(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(&GetAnyProcAddress))) {
		std::cout << "Failed to load GLAD!" << std::endl;
	}


	init();

	while(!ShouldClose(window)) {

		glViewport(0,0,window.width,window.height);

		display();

		SwapBuffers(window);
	}

	return 0;
}
