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

		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<IndexBuffer> QuadIB;
		Ref<Shader> QuadShader;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::mat4 ViewProjection;
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

		// TEMP: Simple hardcoded shader for now
		s_Data.QuadShader = Shader::Create("Renderer2D_Quad", "assets/Shaders/Quad.vert.glsl", "assets/Shaders/Quad.frag.glsl");
		NGN_CORE_INFO("Renderer2D initialized....");
	}

	// Reset state / bind camera
	void Renderer2D::BeginScene(const glm::mat4& vpMatrix)
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.QuadShader->Bind();
		// Temp: Create single function for setting data here
		s_Data.QuadShader->SetMat4(
			"u_ViewProjection",
			vpMatrix
		);

		NGN_CORE_INFO("Renderer2D::BeginScene called. Test ViewProjection:\n{0}", glm::to_string(vpMatrix));
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

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position =
				transform * quadPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->EntityID = id;

			NGN_CORE_INFO("quadPos[{0}] = {1}", i, glm::to_string(s_Data.QuadVertexBufferPtr->Position));

			NGN_CORE_INFO("Vertex {0}: Pos={1}, Color={2}, ID={3}",
				i,
				glm::to_string(s_Data.QuadVertexBufferPtr->Position),
				glm::to_string(s_Data.QuadVertexBufferPtr->Color),
				s_Data.QuadVertexBufferPtr->EntityID
			);

			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::EndScene()
	{
		// TODO: Replicate for circles + lines
		//if (s_Data.QuadIndexCount == 0)
		//	return;

		//uint32_t dataSize =
		//	(uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;

		//// DEBUG
		//NGN_CORE_INFO("Renderer2D::EndScene. QuadIndexCount={0}, dataSize={1} bytes", s_Data.QuadIndexCount, dataSize);
		//size_t vertexCount = dataSize / sizeof(QuadVertex);
		//NGN_CORE_INFO("Uploading {0} vertices to the GPU", vertexCount);

		//for (size_t i = 0; i < std::min<size_t>(vertexCount, 8ul); i++)  // show first 8 vertices max
		//{
		//	const QuadVertex& v = s_Data.QuadVertexBufferBase[i];
		//	NGN_CORE_INFO("Vertex {0}: Pos={1}, Color={2}, ID={3}",
		//		i,
		//		glm::to_string(v.Position),
		//		glm::to_string(v.Color),
		//		v.EntityID
		//	);
		//}

		//s_Data.QuadVA->Bind();
		//s_Data.QuadVB->Bind();
		//s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

		//s_Data.QuadShader->Bind();
		//RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);

		Renderer2D::Flush();

		// Reset
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			
			s_Data.QuadVA->Bind();
			s_Data.QuadVB->Bind(); 
			s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.QuadIndexCount);
		}
	}
}