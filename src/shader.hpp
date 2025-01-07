#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <span>

struct ShaderSource {
	GLenum type;
	const char * text;
};

GLuint loadShaders(std::span<ShaderSource const> shaderSources);

#endif
