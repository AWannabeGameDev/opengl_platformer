#include "renderer/renderer.hpp"

Renderer::Renderer(int width, int height, std::string_view title) :
    _window {createWindow(width, height, title)}, _width {width}, _height {height}
{
    glEnable(GL_DEBUG_OUTPUT);
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);

    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _MAX_INDICES * sizeof(Index), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &_instanceVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _instanceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _MAX_INSTANCES * sizeof(ModelData), nullptr, GL_DYNAMIC_DRAW);

    glBindVertexBuffer(_BINDING_POINT_VERTICES, _vertexBuffer, 0, sizeof(Vertex));
    glBindVertexBuffer(_BINDING_POINT_INSTANCE, _instanceVertexBuffer, 0, sizeof(ModelData));
    glVertexBindingDivisor(_BINDING_POINT_INSTANCE, 1);

    glVertexAttribFormat(_ATTRIB_IDX_POSITION, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexAttribBinding(_ATTRIB_IDX_POSITION, _BINDING_POINT_VERTICES);
    glEnableVertexAttribArray(_ATTRIB_IDX_POSITION);

    glVertexAttribFormat(_ATTRIB_IDX_COLOR, 4, GL_FLOAT, GL_FALSE, offsetof(ModelData, color));
    glVertexAttribBinding(_ATTRIB_IDX_COLOR, _BINDING_POINT_INSTANCE);
    glEnableVertexAttribArray(_ATTRIB_IDX_COLOR);

    for(GLuint i {0}; i < 4; i++)
    {
        glVertexAttribFormat(_ATTRIB_IDX_TRANSFORM + i, 4, GL_FLOAT, GL_FALSE, 
                                offsetof(ModelData, transform) + (i * sizeof(glm::vec4)));
        glVertexAttribBinding(_ATTRIB_IDX_TRANSFORM + i, _BINDING_POINT_INSTANCE);
        glEnableVertexAttribArray(_ATTRIB_IDX_TRANSFORM + i);
    }

    glGenBuffers(1, &_drawCmdBuffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _drawCmdBuffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, _MAX_DRAW_CALLS * sizeof(_DrawCmd), nullptr, GL_STREAM_DRAW);

    _uniforms.addUniform(_defaultShader, "view");
    _uniforms.addUniform(_defaultShader, "projection");
}

Renderer::~Renderer()
{
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_indexBuffer);
    glDeleteBuffers(1, &_instanceVertexBuffer);
    glDeleteBuffers(1, &_drawCmdBuffer);
    glDeleteVertexArrays(1, &_vertexArray);

    glfwTerminate();
}

bool Renderer::userExitedWindow()
{
    return glfwWindowShouldClose(_window);
}

void Renderer::submitModel(const Vertex* vertices, size_t vertexCount, const Index* indices, size_t indexCount, 
                           const ModelData& modelData)
{
    _DrawCmd newDrawCall
    {
        .indexCount {(GLuint)indexCount},
        .instanceCount {1},
        .indexOffset {_totalIndexCount},
        .vertexOffset {(GLint)_totalVertexCount},
        .instanceOffset {_totalInstanceCount}
    };

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _drawCmdBuffer);
    glBufferSubData(GL_DRAW_INDIRECT_BUFFER, _totalDrawCmdCount * sizeof(_DrawCmd), sizeof(_DrawCmd), &newDrawCall);
    _totalDrawCmdCount++;

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, _totalVertexCount * sizeof(Vertex), vertexCount * sizeof(Vertex), vertices);
    _totalVertexCount += vertexCount;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, _totalIndexCount * sizeof(Index), indexCount * sizeof(Index), indices);
    _totalIndexCount += indexCount;

    glBindBuffer(GL_ARRAY_BUFFER, _instanceVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, _totalInstanceCount * sizeof(ModelData), sizeof(ModelData), &modelData);
    _totalInstanceCount++;
};

void Renderer::drawAll(const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(_defaultShader);
    _uniforms.setUniform(_defaultShader, "view", view);
    _uniforms.setUniform(_defaultShader, "projection", projection);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (const void*)0, _totalDrawCmdCount, sizeof(_DrawCmd));

    glfwSwapBuffers(_window);

    #if 0
    // orphan buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _MAX_INDICES * sizeof(Index), nullptr, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _MAX_VERTICES * sizeof(Vertex), nullptr, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _instanceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _MAX_INSTANCES * sizeof(ModelData), nullptr, GL_STREAM_DRAW);

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _drawCmdBuffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, _MAX_DRAW_CALLS * sizeof(_DrawCmd), nullptr, GL_STREAM_DRAW);
    #endif

    _totalVertexCount = 0;
    _totalIndexCount = 0;
    _totalInstanceCount = 0;
    _totalDrawCmdCount = 0;
}