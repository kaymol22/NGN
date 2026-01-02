#include "AppLayer.h"
#include "VoidLayer.h"

#include "Core/Application.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <filesystem>

#include <print>

AppLayer::AppLayer()
{
	std::println("Created new AppLayer!");
	
	// Create shaders
	m_Shader = Renderer::CreateGraphicsShader("Shaders/Fullscreen.vert.glsl", "Shaders/Flame.glsl");

	// Create geometry
	glCreateVertexArrays(1, &m_VertexArray);
	glCreateBuffers(1, &m_VertexBuffer);

	struct Vertex
	{
		glm::vec2 Position;
		glm::vec2 TexCoord;
	};

	Vertex vertices[] = {
		{ {-1.0f, -1.0f}, {0.0f, 0.0f} },	// Bottom left
		{ { 3.0f,-1.0f}, {1.0f, 0.0f} },	// Bottom right
		{ {-1.0f, 3.0f}, {0.0f, 1.0f} }		// Top left
	};

	glNamedBufferData(m_VertexBuffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// Bind VBO to VAO at binding index 0
	glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, sizeof(Vertex));

	//// Enable attributes
	glEnableVertexArrayAttrib(m_VertexArray, 0);	// position
	glEnableVertexArrayAttrib(m_VertexArray, 1);	// uv

	//// Format: location, size, type, normalized, relative offset
	glVertexArrayAttribFormat(m_VertexArray, 0, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, Position)));
	glVertexArrayAttribFormat(m_VertexArray, 1, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, TexCoord)));
}

AppLayer::~AppLayer()
{
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_VertexBuffer);

	glDeleteProgram(m_Shader);
}

void AppLayer::OnEvent(NGN::Event& event)
{
	NGN::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<NGN::MouseButtonPressedEvent>([this](NGN::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<NGN::MouseMovedEvent>([this](NGN::MouseMovedEvent& e) { return OnMouseMoved(e); });
	dispatcher.Dispatch<NGN::WindowCloseEvent>([this](NGN::WindowCloseEvent& e) { return OnWindowClosed(e); });
}

void AppLayer::OnUpdate(float timeStep)
{
	m_Time += timeStep;

	if (glfwGetKey(NGN::Application::Get().GetWindow().GetHandle(), GLFW_KEY_1) == GLFW_PRESS)
	{
		TransitionTo<VoidLayer>();
	}
}

void AppLayer::OnRender()
{
	glUseProgram(m_Shader);

	// Uniforms
	glUniform1f(0, m_Time);

	glm::vec2 framebufferSize = NGN::Application::Get().GetFramebufferSize();
	glUniform2f(1, framebufferSize.x, framebufferSize.y);

	glUniform2f(2, m_FlamePosition.x, m_FlamePosition.y);

	glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

	// Render
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(m_VertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

bool AppLayer::OnMouseButtonPressed(NGN::MouseButtonPressedEvent& event)
{
	glm::vec2 framebufferSize = NGN::Application::Get().GetFramebufferSize();
	float aspectRatio = framebufferSize.x / framebufferSize.y;
	glm::vec2 normalizedMousePos = (m_MousePosition / framebufferSize) * 2.0f - 1.0f;
	normalizedMousePos.x *= aspectRatio;
	normalizedMousePos.y *= -1.0f;
	normalizedMousePos.y += 0.7f;

	m_FlamePosition = -normalizedMousePos;

	return false;
}

bool AppLayer::OnMouseMoved(NGN::MouseMovedEvent& event)
{
	m_MousePosition = { static_cast<float>(event.GetX()), static_cast<float>(event.GetY()) };

	return false;
}

bool AppLayer::OnWindowClosed(NGN::WindowCloseEvent& event)
{
	std::println("Window Closed!");

	return false;
}