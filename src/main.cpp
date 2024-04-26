#include <iostream>
#include "window.h"
#include <glad/glad.h>


int main(int argc, const char * argv[]) {
	std::cout << "Hello World!" << std::endl;


	Window window(800, 600, "Hello World!");

	window.MakeCurrentContext();

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(&Window::GetAnyProcAddress))) {
		std::cout << "Failed to load GLAD!" << std::endl;
	}

	glViewport(0,0,800,600);

	while(!window.ShouldClose()) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		window.SwapBuffers();
		
	}

	return 0;
}
