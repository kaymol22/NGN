#include "Viewport.h"
#include "Core/Application.h"
#include "Input/Input.h"
#include "Math/Math.h"

namespace NGN
{
	Viewport::Viewport(uint32_t viewportIndex, const glm::vec2& position, const glm::vec2& size, bool IsOrthographic) 
		: m_Position(position),
		m_Size(size),
		m_IsOrthoGraphic(IsOrthographic),
		m_OrthoSize(1.0f),
		m_NearPlane(0.1f),
		m_FarPlane(1000.0f),
		m_FOV(1.0f),
		m_PerspectiveMatrix(glm::mat4(1.0f)),
		m_OrthoMatrix(glm::mat4(1.0f)),
		m_ViewportMode(ShadingMode::SHADED)
	{
		m_ViewportIndex = viewportIndex;
		UpdateProjectionMatrices();
	}

	void Viewport::UpdateSpaceCoords(SpaceCoords& spaceCoords, uint32_t fullResolutionWidth, uint32_t fullResolutionHeight) {
		spaceCoords.width = fullResolutionWidth * m_Size.x;
		spaceCoords.height = fullResolutionWidth * m_Size.y;
		spaceCoords.localMouseX = NGN::Math::MapRange(NGN::Input::GetMousePosition().first, m_LeftPixel, m_RightPixel, 0, spaceCoords.width);
		spaceCoords.localMouseY = NGN::Math::MapRange(NGN::Input::GetMousePosition().second, m_TopPixel, m_BottomPixel, 0, spaceCoords.height);
	}

	void Viewport::Update() {
		const uint32_t windowWidth = Application::Get().GetFramebufferSize().x;
		const uint32_t windowHeight = Application::Get().GetFramebufferSize().y;

		// Pixel bounds
		m_LeftPixel = m_Position.x * windowWidth;
		m_RightPixel = m_LeftPixel + m_Size.x * windowWidth;
		m_TopPixel = m_Position.y * windowHeight;
		m_BottomPixel = m_TopPixel + m_Size.y * windowHeight;

		// Space Coords
		const Resolutions resolutions = Application::Get().GetResolutions();
		UpdateSpaceCoords(m_WindowSpaceCoords, windowWidth, windowHeight);
		UpdateSpaceCoords(m_GBufferSpaceCoords, resolutions.gBuffer.x, resolutions.gBuffer.y);

		m_HasHover = (
			NGN::Input::GetMousePosition().first >= m_LeftPixel &&
			NGN::Input::GetMousePosition().first < m_RightPixel &&
			NGN::Input::GetMousePosition().second >= m_TopPixel &&
			NGN::Input::GetMousePosition().first < m_BottomPixel
		);
	}

	void Viewport::SetPerspective(float fov, float nearPlane, float farPlane) {
		m_IsOrthoGraphic = false;
		m_FOV = fov;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		UpdateProjectionMatrices();
	}

	void Viewport::SetOrthographic(float orthoSize, float nearPlane, float farPlane) {
		m_IsOrthoGraphic = true;
		m_OrthoSize = orthoSize;
		m_NearPlane = nearPlane;
		m_FarPlane = farPlane;
		UpdateProjectionMatrices();
	}

	void Viewport::UpdateProjectionMatrices() {

	}
}