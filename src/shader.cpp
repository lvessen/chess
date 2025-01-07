#include "shader.hpp"

#include <glad/glad.h>
#include <span>

GLuint createShader(const ShaderSource& shaderSource) {
	GLuint shader = glCreateShader(shaderSource.type);

	glShaderSource(shader, 1, &shaderSource.text, 0); 
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success) {
		//char infoLog[512];
		//glGetShaderInfoLog(shader, 512, NULL, infoLog);
		//std::cout << infoLog << std::endl;
	}

	return shader;
}

GLuint loadShaders(std::span<ShaderSource const> shaderSources) {
	GLuint shaderProgram = glCreateProgram();

	for (auto shaderSource : shaderSources) {
		const auto shaderId = createShader(shaderSource);
		glAttachShader(shaderProgram, shaderId);
	}

	glLinkProgram(shaderProgram);

	return shaderProgram;
}
