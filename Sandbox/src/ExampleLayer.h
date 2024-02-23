#pragma once
#include "Mashenka.h"

class ExampleLayer : public Mashenka::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer() override;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Mashenka::TimeStep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Mashenka::Event& e) override;
    
private:
    /*m_VertexArray: This is the ID of the Vertex Array Object (VAO).
 *A VAO encapsulates all of the state needed to specify per-vertex attribute data to the OpenGL pipeline.
 *It essentially serves as a container for VBOs and EBOs.
 
m_VertexBuffer: This is the ID of the Vertex Buffer Object (VBO).
A VBO is a memory buffer in the high-speed memory of your video card designed to hold information about vertices.
In a 3D application, these could include coordinates, normals, and texture coordinates.

m_IndexBuffer: This is the ID of the Element Buffer Object (EBO), also known as the Index Buffer Object (IBO).
It stores the indices that OpenGL uses to decide which vertices to draw,
allowing for the reuse of vertex data and thus more efficient rendering.*/

    // Explanation: https://www.khronos.org/opengl/wiki/Shader_Compilation
    // The shader program is the final linked version of multiple shaders combined.
    // The shader program is the one that is used in the rendering process.
    // The shader program is the one that is stored in the OpenGL state machine.

    // Example data for the triangle: VAO and shader
    Mashenka::ShaderLibrary m_ShaderLibrary;
    Mashenka::Ref<Mashenka::Shader> m_Shader;
    Mashenka::Ref<Mashenka::VertexArray> m_VertexArray;
    glm::vec3 m_TrianglePosition = {0.0f, 0.0f, 0.0f};

    // Example data for blue square: VAO and shader
    Mashenka::Ref<Mashenka::Shader> m_FlatColorShader;
    Mashenka::Ref<Mashenka::VertexArray> m_SquareVA;
    glm::vec3 m_SquarePosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};

    // Example data for texture
    Mashenka::Ref<Mashenka::Texture2D> m_Texture;
    Mashenka::Ref<Mashenka::Texture2D> m_ChernoLogoTexture;


    // Camera and properties
    // Why Define the camera here? Because the camera is a part of the layer, and the layer is a part of the application
    // The camera is a part of the layer because the camera is a part of the scene
    Mashenka::OrthographicCameraController m_CameraController;
};
