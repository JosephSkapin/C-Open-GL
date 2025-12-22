cmake_minimum_required(VERSION 3.16)
project(ogl_engine_test LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(glfw3 REQUIRED)
add_executable(ogl_engine_test
	src/main.cpp
	external/glad/src/glad.c
)
target_include_directories(ogl_engine_test PRIVATE
	external/glad/include
)

target_link_libraries(ogl_engine_test PRIVATE 
	glfw
	GL
	dl
)

