#include <stdexcept>

#include "renderer/renderer.hpp"

void Renderer::_createWindow(std::string_view title)
{
    if(glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error {"Failed to initialize GLFW."};
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    _window = glfwCreateWindow(_width, _height, title.data(), nullptr, nullptr);

    if(not _window)
    {
        glfwTerminate();
        throw std::runtime_error {"Failed to create window."};
    }

    glfwMakeContextCurrent(_window);

    if(not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        throw std::runtime_error {"Failed to initialize GLAD."};
    }

    printf("Using OpenGL version : %s\n", glGetString(GL_VERSION));
}

void Renderer::_setupVertexArray()
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindVertexBuffer(_BINDING_POINT_VERTICES, _vbo, 0, sizeof(Vertex));

    glVertexAttribFormat(_ATTRIB_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexAttribBinding(_ATTRIB_INDEX_POSITION, _BINDING_POINT_VERTICES);
    glEnableVertexAttribArray(_ATTRIB_INDEX_POSITION);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
}

Renderer::Renderer(int windowWidth, int windowHeight, std::string_view title) :
    _width {windowWidth}, _height {windowHeight}
{
    _createWindow(title);
    _setupVertexArray();
}

Renderer::~Renderer()
{
    glfwTerminate();
}

bool Renderer::userExitedWindow()
{
    return glfwWindowShouldClose(_window);
}

size_t Renderer::newPrefab(const Vertex* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount)
{
    if(sceneOpen)
    {
        throw std::logic_error {"Can't create prefab while scene is open."};    
    }

    _vertices.insert(_vertices.end(), vertices, vertices + vertexCount);
    _indices.insert(_indices.end(), indices, indices + indexCount);
}

void Renderer::beginScene()
{
    if(_vertices.size() > _vboSize)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_DYNAMIC_DRAW);
    }

    if(_indices.size() > _eboSize)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_DYNAMIC_DRAW);
    }

    sceneOpen = true;
}

void Renderer::endScene()
{
    sceneOpen = false;
}

void Renderer::drawAll()
{
    glfwSwapBuffers(_window);
}