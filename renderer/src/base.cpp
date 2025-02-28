#include "renderer/base.hpp"
#include <stdio.h> 
#include <fstream>
#include <sstream>

GLFWwindow* createWindow(int windowWidth, int windowHeight, std::string_view title)
{
	if(glfwInit() != GLFW_TRUE)
	{
		printf("Failed to initialize GLFW\n");
		return nullptr;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window {glfwCreateWindow(windowWidth, windowHeight, title.data(), nullptr, nullptr)};
	if(!window)
	{
		printf("Failed to create window\n");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return nullptr;
	}
	printf("Using OpenGL version : %s\n", glGetString(GL_VERSION));

	return window;
}

unsigned int compileShader(unsigned int type, std::string_view path)
{
	std::ifstream file {};
	file.open(path.data());
	std::stringstream sstream {};
	std::string source {};

	if(!file.is_open())
	{
		printf("Failed to open shader file %s.\n", path.data());
		return 0;
	} 
	else
	{
		sstream << file.rdbuf();
		source = sstream.str();
	}

	const char* sourceStr {source.c_str()};

	unsigned int shader {glCreateShader(type)};
	glShaderSource(shader, 1, &sourceStr, nullptr);
	glCompileShader(shader);

#ifdef _DEBUG

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(success != GL_TRUE)
	{
		int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		char* message {(char*)::operator new(logLength)};
		glGetShaderInfoLog(shader, logLength, nullptr, message);

		printf("Failed to compile shader %s : %s", path.data(), message);

		::operator delete(message);

		return 0;
	}

#endif

	return shader;
}

unsigned int createShaderProgram(const std::initializer_list<std::string_view>& shaderPaths)
{
	unsigned int program {glCreateProgram()};

	for(const std::string_view& path : shaderPaths)
	{
		std::string_view extension {path.substr(path.size() - 3, 3)};
		int extensionHash {extension[0] + (2 * extension[1]) + (3 * extension[2])};

		unsigned int shaderType;

		switch(extensionHash)
		{
			case 'v' + (2 * 'x') + (3 * 's') : shaderType = GL_VERTEX_SHADER; break;
			case 'g' + (2 * 'm') + (3 * 's') : shaderType = GL_GEOMETRY_SHADER; break;
			case 'f' + (2 * 'm') + (3 * 's') : shaderType = GL_FRAGMENT_SHADER; break;
			case 'c' + (2 * 'p') + (3 * 's') : shaderType = GL_COMPUTE_SHADER; break;
		}

		unsigned int shader = compileShader(shaderType, path);
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}

	glLinkProgram(program);

	#ifdef _DEBUG

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if(success != GL_TRUE)
	{
		int logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		char* message {(char*)::operator new(logLength)};
		glGetProgramInfoLog(program, logLength, nullptr, message);

		printf("Failed to link program : %s", message);

		::operator delete(message);

		return 0;
	}

	#endif

	return program;
}