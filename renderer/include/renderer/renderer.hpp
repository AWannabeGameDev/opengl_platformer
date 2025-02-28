#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>

#include <stdexcept>

#include "renderer/base.hpp"

class Renderer
{
private :
    struct _DrawCmd
    {
        GLuint indexCount;
        GLuint instanceCount;
        GLuint indexOffset;
        GLint vertexOffset;
        GLuint instanceOffset;
    };

public :
    using Index = unsigned int;

    struct Vertex
    {   
        glm::vec3 position;
    };

    struct ModelData
    {
        glm::mat4 transform;
        glm::vec4 color;
    };

private :
    static constexpr size_t _MAX_VERTICES {1024};
    static constexpr size_t _MAX_INDICES {4096};
    static constexpr size_t _MAX_INSTANCES {1024};
    static constexpr size_t _MAX_DRAW_CALLS {1024};

    static constexpr GLuint _BINDING_POINT_VERTICES {0};
    static constexpr GLuint _BINDING_POINT_INSTANCE {1};

    static constexpr GLuint _ATTRIB_IDX_POSITION {0};
    static constexpr GLuint _ATTRIB_IDX_TRANSFORM {1};
    static constexpr GLuint _ATTRIB_IDX_COLOR {5};

    GLFWwindow* _window;
    int _width;
    int _height;

    GLuint _vertexBuffer;
    GLuint _instanceVertexBuffer;
    GLuint _indexBuffer;
    GLuint _drawCmdBuffer;
    GLuint _vertexArray;

    GLuint _totalVertexCount {0};
    GLuint _totalInstanceCount {0};
    GLuint _totalIndexCount {0};
    GLuint _totalDrawCmdCount {0};

    GLuint _defaultShader {createShaderProgram({"../../renderer/src/shaders/default.vxs",
                                               "../../renderer/src/shaders/default.fms"})};

public :
    Renderer(int width, int height, std::string_view title);
    ~Renderer();

    bool userExitedWindow();
    
    void submitModel(const Vertex* vertices, size_t vertexCount, const Index* indices, size_t indexCount, 
                     const ModelData& modelData);

    void drawAll();
};

#endif