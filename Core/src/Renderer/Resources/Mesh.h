#pragma once

#include "Buffer.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include <vector>

namespace NGN
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        ~Mesh() = default;

        void Bind() const;
        void Unbind() const;

        uint32_t GetIndexCount() const { return m_IndexCount; }
        const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

        // Bounding box
        glm::vec3 GetBoundsMin() const { return m_BoundsMin; }
        glm::vec3 GetBoundsMax() const { return m_BoundsMax; }

        // Create parametric meshes
        static Ref<Mesh> CreateCube(float size = 1.0f);
        static Ref<Mesh> CreatePyramid(float size = 1.0f);
        static Ref<Mesh> CreatePlane(float width = 1.0f, float height = 1.0f);

    private:
        void ComputeBounds(const std::vector<Vertex>& vertices);

    private:
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        uint32_t m_IndexCount = 0;

        glm::vec3 m_BoundsMin = glm::vec3(0.0f);
        glm::vec3 m_BoundsMax = glm::vec3(0.0f);
    };
}
