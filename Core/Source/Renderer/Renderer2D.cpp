#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Scene/Entity.h"

//TEMP: Debugging
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

namespace NGN
{
	struct QuadVertex
	{
		glm::vec4 Position;
		glm::vec4 Color;
		int EntityID;
	};

	struct Renderer2DData
	{
		static constexpr uint32_t MaxQuads = 1000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<IndexBuffer> QuadIB;
		Ref<Shader> QuadShader;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::mat4 ViewProjection;
		Renderer2D::Renderer2DStats Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data.QuadVA = VertexArray::Create();
		// Need to have Vertex Array bound in order to set index buffer
		s_Data.QuadVA->Bind();

		// Vertex Buffer dynamic setup
		s_Data.QuadVB = VertexBuffer::Create(
			s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVB->SetLayout({
			{ ShaderDataType::Float4,	"a_Position" },
			{ ShaderDataType::Float4,	"a_Color" },
			{ ShaderDataType::Int,		"a_EntityID" }
		});

		// Index Buffer
		uint32_t* indices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);
		s_Data.QuadIB = IndexBuffer::Create(indices, s_Data.MaxIndices);
		s_Data.QuadIB->Bind();

		s_Data.QuadVA->SetIndexBuffer(s_Data.QuadIB);
		delete[] indices;

		// CPU side buffer
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		s_Data.QuadShader = Shader::Create("Renderer2D_Quad", "assets/Shaders/Quad.vert.glsl", "assets/Shaders/Quad.frag.glsl");
	}

	// Reset state / bind camera
	void Renderer2D::BeginScene(const glm::mat4& vpMatrix)
	{
		Renderer2DStats::ResetStats();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.QuadShader->Bind();
		// Temp: Create single function for setting data here
		s_Data.QuadShader->SetMat4(
			"u_ViewProjection",
			vpMatrix
		);


	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, EntityID id)
	{
		// TODO: Add these uniforms + expand buffer/vertex array classes + QuadVertex struct
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		constexpr glm::vec4 quadPositions[4] =
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		NGN_CORE_INFO("DrawQuad EntityID = {0}, Color = {1}", id, glm::to_string(color));

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position =
				transform * quadPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->EntityID = id;

			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.Stats.QuadCount++;
		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::EndScene()
	{
		Renderer2D::Flush();

		// Reset
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::Flush()
	{
		NGN_CORE_ASSERT(s_Data.QuadIndexCount <= s_Data.MaxIndices, "Renderer2D batch overflow!");

		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			
			// DEBUG
			NGN_CORE_INFO("Flushing {0} quads, {1} indices, {2} bytes",
				s_Data.Stats.QuadCount, s_Data.QuadIndexCount, dataSize);

			// DEBUG: Check first vertex color
			NGN_CORE_INFO("First vertex color: {0}", glm::to_string(s_Data.QuadVertexBufferBase[0].Color));

			s_Data.QuadVA->Bind();
			s_Data.QuadVB->Bind(); 
			s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);

			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::Renderer2DStats::ResetStats()
	{
		s_Data.Stats = {};
	}

	Renderer2D::Renderer2DStats Renderer2D::Renderer2DStats::GetStats()
	{
		return s_Data.Stats;
	}
}