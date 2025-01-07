#include <iostream>

#include <glad/glad.h>
#include <array>
#include "chessboard.hpp"
#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDS { vPosition = 0, vTexture = 1};
unsigned int texture;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];


constexpr static GLuint NumVertices = 6;

const char * vertSource = R"(#version 430 core
layout (location = 0) in vec4 pos;
out vec2 TexCoord;
void main() {
	TexCoord = vec2(pos.xy);
	gl_Position = vec4(-0.5,-0.5,0.0,0.0) + pos;
})";
const char * fragSource = R"( #version 430 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main() {
	FragColor = texture(ourTexture, TexCoord);
})";

void LoadTextures()
{
stbi_set_flip_vertically_on_load(true);  
    int x,y,n;
    unsigned char *data = stbi_load(".\\assets\\wood4.jpg", &x, &y, &n, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

    stbi_image_free(data);

}
void init()
{
	static const GLfloat vertices[NumVertices][4] = {
		{ 0.0, 0.0, 0.0f, 0.0f},
		{ 1.0, 0.0, 1.0f, 0.0f},
		{ 0.0, 1.0, 0.0f, 1.0f},

		{ 1.0, 0.0, 1.0f, 0.0f},
		{ 1.0, 1.0, 1.0f, 1.0f},
		{ 0.0, 1.0, 0.0f, 1.0f}
	};

	glCreateBuffers(NumBuffers, Buffers);
	glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

	GLuint program = loadShaders(std::array<ShaderSource, 2>{{{GL_VERTEX_SHADER, vertSource}, {GL_FRAGMENT_SHADER, fragSource}}});
	glUseProgram(program);

	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(vPosition);

	glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(vTexture);

	LoadTextures();
}

void display()
{
	const static float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
	glClearBufferfv(GL_COLOR, 0, black);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); 
}

