#include <glad/glad.h>
#include<GLFW/glfw3.h>

#include <cstdio>
#include <iostream>

static const char* vsSrc = R"GLSL(
#version 330 core
layout(location=0) in vec3 aPos;
void main(){ gl_Position = vec4(aPos, 1.0); }
)GLSL";

static const char* fsSrc = R"GLSL(
#version 330 core
out vec4 FragColor;
void main(){ FragColor = vec4(0.9, 0.4, 0.2, 1.0); }
)GLSL";

static GLuint compileShader(GLenum type, const char* src){
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if(!ok){
        char log[1024]; glGetShaderInfoLog(s, 1024, nullptr, log);
        std::fprintf(stderr, "Shader compile error: %s\n", log);
        std::exit(1);
    }
    return s;
}
	void framebuffer_size_callback(GLFWwindow* window, int width , int height);
	
	void framebuffer_size_callback(GLFWwindow* window, int width , int height)
	{
		glViewport(0,0,width,height);
	}
	
void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glViewport(0,0,800,600); //sets up the window size and starting coordinates.
	
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);//recursive call to call the function on every window resize
	
	
	
	while(!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);
		
		//rendering commands here...
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	
	return 0;
/*
	if(!glfwInit()) return 1;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	if(glewInit()!=GLEW_OK){
        std::fprintf(stderr, "Failed to init GLEW\n");
        return 1;
    }
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* win = glfwCreateWindow(1280,720, "Triangle", nullptr, nullptr);
	if(!win){glfwTerminate(); return 1;}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(1);
	
	glewExperimental = GL_TRUE;

    std::printf("Renderer: %s\n", glGetString(GL_RENDERER));
    std::printf("OpenGL:   %s\n", glGetString(GL_VERSION));	
    
    float verts[] = {
        -0.6f, -0.4f, 0.0f,
         0.6f, -0.4f, 0.0f,
         0.0f,  0.6f, 0.0f
    };
    
    GLuint vao=0, vbo=0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSrc);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    while(!glfwWindowShouldClose(win)){
        glfwPollEvents();
        glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(win);
    }

    glDeleteProgram(prog);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
    
*/ 
}
