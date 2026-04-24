#include "Mesh.h"
#include "VertexArray.h"

namespace NGN
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
        : m_IndexCount(static_cast<uint32_t>(indices.size()))
    {
        m_VertexArray = VertexArray::Create();

        // Create vertex buffer
        m_VertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(Vertex));
		m_VertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(Vertex));

        // Setup vertex layout
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };
        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        // Create index buffer
        m_IndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        // Compute bounding box
        ComputeBounds(vertices);
    }

    void Mesh::Bind() const
    {
        m_VertexArray->Bind();
    }

    void Mesh::Unbind() const
    {
        m_VertexArray->Unbind();
    }

    void Mesh::ComputeBounds(const std::vector<Vertex>& vertices)
    {
        if (vertices.empty())
            return;

        m_BoundsMin = vertices[0].Position;
        m_BoundsMax = vertices[0].Position;

        for (const auto& vertex : vertices)
        {
            m_BoundsMin = glm::min(m_BoundsMin, vertex.Position);
            m_BoundsMax = glm::max(m_BoundsMax, vertex.Position);
        }
    }

    Ref<Mesh> Mesh::CreateCube(float size)
    {
        float s = size * 0.5f;

        std::vector<Vertex> vertices = {
            // Front face
            { {-s, -s,  s}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
            { { s, -s,  s}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
            { { s,  s,  s}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
            { {-s,  s,  s}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },

            // Back face
            { { s, -s, -s}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },
            { {-s, -s, -s}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} },
            { {-s,  s, -s}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} },
            { { s,  s, -s}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} },

            // Right face
            { { s, -s,  s}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { { s, -s, -s}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
            { { s,  s, -s}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
            { { s,  s,  s}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },

            // Left face
            { {-s, -s, -s}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
            { {-s, -s,  s}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
            { {-s,  s,  s}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },
            { {-s,  s, -s}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },

            // Top face
            { {-s,  s,  s}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
            { { s,  s,  s}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { { s,  s, -s}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
            { {-s,  s, -s}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },

            // Bottom face
            { {-s, -s, -s}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f} },
            { { s, -s, -s}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} },
            { { s, -s,  s}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} },
            { {-s, -s,  s}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },
        };

        std::vector<uint32_t> indices = {
            // Front
            0, 1, 2, 2, 3, 0,
            // Back
            4, 5, 6, 6, 7, 4,
            // Right
            8, 9, 10, 10, 11, 8,
            // Left
            12, 13, 14, 14, 15, 12,
            // Top
            16, 17, 18, 18, 19, 16,
            // Bottom
            20, 21, 22, 22, 23, 20
        };

        return CreateRef<Mesh>(vertices, indices);
    }

    Ref<Mesh> Mesh::CreatePyramid(float size)
    {
        float s = size * 0.5f;

        std::vector<Vertex> vertices = {
            // Base
            { {-s, -s, -s}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f} },
            { { s, -s, -s}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} },
            { { s, -s,  s}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} },
            { {-s, -s,  s}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },
            // Apex
            { {0.0f, s, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.5f} },
        };

        std::vector<uint32_t> indices = {
            // Base
            0, 1, 2, 2, 3, 0,
            // Sides
            0, 4, 1,
            1, 4, 2,
            2, 4, 3,
            3, 4, 0
        };

        return CreateRef<Mesh>(vertices, indices);
    }

    Ref<Mesh> Mesh::CreatePlane(float width, float height)
    {
        float w = width * 0.5f;
        float h = height * 0.5f;

        std::vector<Vertex> vertices = {
            { {-w, 0.0f, -h}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
            { { w, 0.0f, -h}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
            { { w, 0.0f,  h}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
            { {-w, 0.0f,  h}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
        };

        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        return CreateRef<Mesh>(vertices, indices);
    }
}
