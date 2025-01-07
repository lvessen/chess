cmake_minimum_required(VERSION 3.11)
project(chess VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(chess src/main.cpp 
					 src/window.cpp 
					 src/chessboard.cpp 
					 src/shader.cpp 
					 src/glad.c)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

target_link_libraries(chess PRIVATE glm::glm)
target_link_libraries(chess PUBLIC opengl32.lib)
target_include_directories(chess PUBLIC include)


