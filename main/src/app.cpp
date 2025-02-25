#include <renderer/renderer.hpp>

#include <array>

int main()
{
    Renderer render {1280, 720, "OpenGL Platformer"};

    std::array<Renderer::Vertex, 3> triVerts
    {
        Renderer::Vertex {glm::vec3 {0.0f, 0.5f, 0.0f}},
        Renderer::Vertex {glm::vec3 {-0.5f, -0.5f, 0.0f}},
        Renderer::Vertex {glm::vec3 {-0.5f, 0.5f, 0.0f}}
    };

    size_t triPrefabId {render.newPrefab(triVerts.data(), triVerts.size())};

    while(not render.userExitedWindow())
    {
        glfwPollEvents();
        render.drawAll();
    }
}