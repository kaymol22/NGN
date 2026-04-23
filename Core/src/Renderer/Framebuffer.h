#pragma once

#include "Core/Base.h"

#include "Texture.h"

namespace NGN
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8, // Base Color
		RED_INTEGER, // Handy for ID picking
		DEPTH24STENCIL8,// Depth/Stencil
		Depth = DEPTH24STENCIL8,

		// G-buffer attachments
		RGB16F, // World space positions
		RGB10A2, // Packed normals
		SRGB8, // Albedo
		RG16F, // Material params - metal, roughness, ao
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// Filtering/wrap modes here if needed
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;
		// Keep fb versatile - need bool for renderpass functionality
		// false = render to texture
		bool SwapChainTarget = false;

		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		// Mouse selection + framebuffer debugging
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}