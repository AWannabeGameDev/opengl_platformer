#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <string_view>
#include <stdio.h>
#include <vector>

class Renderer
{
public :
    struct Vertex
    {
        glm::vec3 position;
    };

private :
    static constexpr unsigned int _BINDING_POINT_VERTICES = 0;

    static constexpr unsigned int _ATTRIB_INDEX_POSITION = 0;

    GLFWwindow* _window;
    int _width;
    int _height;

    bool sceneOpen {false};

    unsigned int _vbo;
    size_t _vboSize {0};
    unsigned int _ebo;
    size_t _eboSize {0};
    unsigned int _vao;

    std::vector<Vertex> _vertices {};
    std::vector<unsigned int> _indices {};

    void _createWindow(std::string_view title);
    void _setupVertexArray();

public :

    Renderer(int windowWidth, int windowHeight, std::string_view title);
    ~Renderer();

    bool userExitedWindow();

    size_t newPrefab(const Vertex* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);

    void beginScene();
    void endScene();
    void drawAll();
};

#endif