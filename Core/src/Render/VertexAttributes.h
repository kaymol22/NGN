#pragma once

#include <span>

enum class VertexAttributeType {
	Float,
	Int, 
	UnsignedInt
};

struct VertexAttribute {
	uint32_t location = 0;
	uint32_t componentCount = 0;
	VertexAttributeType type = VertexAttributeType::Float;
	bool normalized = false;
	size_t offset = 0;
};

struct VertexLayoutDescription {
	size_t stride = 0;
	std::span<const VertexAttribute> attributes;
};

struct Vertex {
	glm::vec3 position = glm::vec3(0);
	glm::vec3 normal = glm::vec3(0);
	glm::vec2 uv = glm::vec2(0);
	glm::vec3 tangent = glm::vec3(0);

	static VertexLayoutDescription GetLayout() {
		static constexpr std::array<VertexAttribute, 4> attributes = {
			VertexAttribute { 0, 3, VertexAttributeType::Float, false, offsetof(Vertex, position) },
			VertexAttribute { 1, 3, VertexAttributeType::Float, false, offsetof(Vertex, normal) },
			VertexAttribute { 2, 2, VertexAttributeType::Float, false, offsetof(Vertex, uv) },
			VertexAttribute { 3, 3, VertexAttributeType::Float, false, offsetof(Vertex, tangent) }
		};
		return { sizeof(Vertex), attributes };
	}

	static VertexLayoutDescription GetPositionUVLayout() {
		static constexpr std::array<VertexAttribute, 4> attributes = {
			VertexAttribute { 0, 3, VertexAttributeType::Float, false, offsetof(Vertex, position) },
			VertexAttribute { 2, 2, VertexAttributeType::Float, false, offsetof(Vertex, uv) }
		};
		return { sizeof(Vertex), attributes };
	}
};