#include <iostream>

#include <glad/glad.h>
#include "triangles.hpp"

#include <fstream>
#include <iterator>
#include <algorithm>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDS { vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];


constexpr static GLuint NumVertices = 6;

struct ShaderInfo
{
	GLenum shaderType;
	const char * fileName;
};

GLuint CreateShader(const ShaderInfo& shaderInfo)
{
	GLuint shader = glCreateShader(shaderInfo.shaderType);
	std::ifstream file{shaderInfo.fileName};

	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);


	std::string tmp;
	tmp.resize(length);
	file.read(tmp.data(), length);

	std::cout << tmp <<std::endl;
	const char * t = tmp.c_str();

	glShaderSource(shader, 1, &t, 0); 
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	return shader;
}

GLuint LoadShaders(ShaderInfo const * shaderInfo)
{
	GLuint shaderProgram = glCreateProgram();

	while(shaderInfo->shaderType != GL_NONE) {

		const auto shaderId = CreateShader(*shaderInfo);
		glAttachShader(shaderProgram, shaderId);
		shaderInfo++;
	}

	glLinkProgram(shaderProgram);

	return shaderProgram;
}

void init()
{
	static const GLfloat vertices[NumVertices][2] = {
		{-0.90, -0.90},
		{ 0.85, -0.90},
		{-0.90,  0.85},

		{ 0.90, -0.85},
		{ 0.90,  0.90},
		{-0.85,  0.90}
	};

	glCreateBuffers(NumBuffers, Buffers);
	glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

	ShaderInfo shaders[] = {{GL_VERTEX_SHADER, "triangles.vert"}, {GL_FRAGMENT_SHADER, "triangles.frag"}, {GL_NONE, NULL}};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
}

void display()
{
	const static float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); 
}

