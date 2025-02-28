#include <renderer/renderer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <windows.h>

#include <array>

int main()
{
    Renderer render {1280, 720, "OpenGL Platformer"};

    std::array<Renderer::Vertex, 3> triVerts
    {
        glm::vec3 {0.0f, 0.5f, 0.5f},
        glm::vec3 {-0.5f, -0.5f, 0.5f},
        glm::vec3 {0.5f, -0.5f, 0.5f}
    };

    std::array<Renderer::Index, 3> triInds
    {
        0, 1, 2
    };

    Renderer::ModelData triModelData
    {
        .transform {glm::translate(glm::mat4{1.0f}, glm::vec3{1.0f, 0.0f, 0.0f})},
        .color {0.0f, 1.0f, 1.0f, 1.0f}
    };

    while(not render.userExitedWindow())
    {
        glfwPollEvents();

        render.submitModel(triVerts.data(), triVerts.size(), triInds.data(), triInds.size(), triModelData);

        render.drawAll();
    }

    return 0;
}