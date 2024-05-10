cmake_minimum_required(VERSION 3.10)

project(chess VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(chess src/main.cpp 
					 src/window.cpp 
					 src/triangles.cpp 
					 src/glad.c)

target_link_libraries(chess PUBLIC opengl32.lib)
target_include_directories(chess PUBLIC include)


