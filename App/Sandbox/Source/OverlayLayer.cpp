#include "OverlayLayer.h"

#include "Core/Application.h"

#include "Renderer/Shader.h"

#include "AppLayer.h"
#include "VoidLayer.h"

#include <print>

OverlayLayer::OverlayLayer()
{
	std::println("Created new OverlayLayer");

	m_Shader = Renderer::CreateGraphicsShader("Shaders/Transform.vertex.glsl", "Shaders/Texture.fragment.glsl");

	glCreateVertexArrays(1, &m_VertexArray);
	glCreateBuffers(1, &m_VertexBuffer);
	glCreateBuffers(1, &m_IndexBuffer);

	struct Vertex {
		glm::vec2 Position;
		glm::vec2 TexCoord;
	};
	// Quad
	Vertex vertices[] = {
		{ {-0.5f,-0.5f}, {0.0f, 0.0f} },	// Bottom left
		{ { 0.5f,-0.5f}, {1.0f, 0.0f} },	// Bottom right
		{ { 0.5f, 0.5f}, {1.0f, 1.0f} },	// Top right
		{ {-0.5f, 0.5f}, {0.0f, 1.0f} }		// Top left
	};

	glNamedBufferData(m_VertexBuffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
	glNamedBufferData(m_IndexBuffer, sizeof(indices), indices, GL_STATIC_DRAW);

	// Bind VBO to VAO at binding index 0
	glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(m_VertexArray, m_IndexBuffer);

	// Enable attributes
	glEnableVertexArrayAttrib(m_VertexArray, 0);
	glEnableVertexArrayAttrib(m_VertexArray, 1);

	//// Format: location, size, type, normalized, relative offset
	glVertexArrayAttribFormat(m_VertexArray, 0, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, Position)));
	glVertexArrayAttribFormat(m_VertexArray, 1, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, TexCoord)));

	// Link attrib locations to binding index 0
	glVertexArrayAttribBinding(m_VertexArray, 0, 0);
	glVertexArrayAttribBinding(m_VertexArray, 1, 0);

	m_Texture = Renderer::LoadTexture("Textures/Button.png");
}

OverlayLayer::~OverlayLayer()
{
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_VertexBuffer);

	glDeleteProgram(m_Shader);

	glDeleteTextures(1, &m_Texture.Handle);
}

void OverlayLayer::OnUpdate(float timeStep)
{
	
}

void OverlayLayer::OnRender()
{

}