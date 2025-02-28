#ifndef BASE_HPP
#define BASE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <initializer_list>

GLFWwindow* createWindow(int windowWidth, int windowHeight, std::string_view title);

unsigned int compileShader(unsigned int type, std::string_view path);
unsigned int createShaderProgram(const std::initializer_list<std::string_view>& shaderPaths);

#endif