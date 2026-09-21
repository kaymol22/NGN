#pragma once
#include "Render/Frustum.h"
#include "Core/Enums.h"

namespace NGN
{
	struct SpaceCoords {
		float width;
		float height;
		float localMouseX;
		float localMouseY;
	};

	struct Viewport {
	public:
		Viewport(uint32_t viewportIndex, const glm::vec2& position = { 0.0f, 0.0f }, const glm::vec2& size = { 1.0f, 1.0f }, bool IsOrthographic = true);
		void Update();

		void SetOrthographic(float orthoSize, float nearPlane, float farPlane);
		void SetPerspective(float fov, float nearPlane, float farPlane);
		void SetPosition(const glm::vec2& position);
		void SetSize(const glm::vec2& size);
		void Show();
		void Hide();
		void SetViewportMode(ShadingMode mode);
		void SetOrthoSize(float value);
		const bool IsVisible() const;
		const bool IsOrthographic() const;
		const bool IsHovered() const;
		const float GetOrthoSize() const;
		const float GetPerspectiveFOV() const;
		glm::vec2 GetPosition() const;
		glm::vec2 GetSize() const;
		glm::mat4 GetProjectionMatrix() const;
		glm::mat4 GetProjectionMatrixReverseZ() const;
		glm::mat4 GetPerspectiveMatirx() const;
		glm::mat4 GetOrthographicMatirx() const;
		glm::vec2 WorldToScreen(const glm::mat4& viewMatirx, const glm::vec3& worldPosition) const;
		glm::ivec2 GetLocalMouseCoords();
		ShadingMode GetViewportMode() const;
		SpaceCoords GetWindowSpaceCoords() const;
		SpaceCoords GetGBufferSpaceCoords() const;

	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;
		bool m_IsOrthoGraphic = false;
		float m_OrthoSize;
		float m_NearPlane;
		float m_FarPlane;
		float m_FOV;
		float m_AspectRatio;
		float m_LeftPixel;
		float m_RightPixel;
		float m_TopPixel;
		float m_BottomPixel;
		bool m_IsVisible = true;
		bool m_HasHover = false;
		uint32_t m_ViewportIndex = 0;
		glm::mat4 m_PerspectiveMatrix;
		glm::mat4 m_PerspectiveMatrixRevZ;
		glm::mat4 m_OrthoMatrix;
		glm::mat4 m_OrthoMatrixRevZ;
		NGN::Frustum m_Frustum;
		ShadingMode m_ViewportMode;
		SpaceCoords m_WindowSpaceCoords;
		SpaceCoords m_GBufferSpaceCoords;

		void UpdateProjectionMatrices();
		void UpdateSpaceCoords(SpaceCoords& spaceCoords, uint32_t fullResolutionWidth, uint32_t fullResolutionHeight);
	};
}