#pragma once
#include "Types/GL_Texture.h"
#include "Types/GL_ssbo.h"
#include "Types/GL_FrameBuffer.h"
#include "Types/GL_Shader.h"
#include "Types/GL_GenericMesh.h"

#include "Containers/SlotMap.h"

/* Resource Types
		CUBEMAP_FRAME_BUFFER,
		CUBEMAP_VIEW,
		FRAME_BUFFER,
		GENERIC_MESH,
		MESH_BUFFER,
		SHADER,
		SHADOW_CUBE_MAP_ARRAY,
		SHADOW_MAP,
		SHADOW_MAP_ARRAY,
		SSBO,
		TEXTURE,
		TEXTURE_3D,
		TEXTURE_ARRAY,
	VK-Specific:
		ACCELERATION_STRUCTURE
	*/
namespace OpenGL::ResourceManager
{
	enum class ResourceType : uint16_t
	{
		Shader = 0,
		FrameBuffer = 1,
		Texture = 2, 
		SSBO = 3,
		GenericMesh = 4
	};
	void Init();
	void CleanUp();

	uint64_t CreateShader(const std::string& name);
	OpenGLShader& LoadShader(const std::string& name, const std::vector<std::string>& paths, const std::vector<std::string>& defines = std::vector<std::string>());
	OpenGLShader& LoadShader(const std::string& subDir, const std::string& name, const std::vector<std::string>& paths, const std::vector<std::string>& defines = std::vector<std::string>());
	OpenGLShader& GetShader(const std::string& name);
	OpenGLShader* GetShaderPtr(const std::string& name);
	OpenGLShader& GetShaderById(uint64_t id);
	OpenGLShader* GetShaderPtrById(uint64_t id);
	void HotloadShaders();
	void RemoveShader(uint64_t id);

	OpenGLFrameBuffer& CreateFrameBuffer(const std::string& name);
	OpenGLFrameBuffer& GetFrameBuffer(const std::string& name);
	OpenGLFrameBuffer* GetFrameBufferPtr(const std::string& name);
	OpenGLFrameBuffer& GetFrameBufferById(uint64_t id);
	OpenGLFrameBuffer* GetFrameBufferPtrById(uint64_t id);
	void RemoveFrameBuffer(uint64_t id);

	OpenGLSSBO& CreateSSBO(const std::string& name);
	OpenGLSSBO& GetSSBO(const std::string& name);
	OpenGLSSBO* GetSSBOPtr(const std::string& name);
	OpenGLSSBO& GetSSBOById(uint64_t id);
	OpenGLSSBO* GetSSBOPtrById(uint64_t id);
	void RemoveSSBO(uint64_t id);
	void RemoveSSBOByName(const std::string& name);

	uint64_t CreateTexture();
	OpenGLTexture& GetTexture(uint64_t id);
	OpenGLTexture* GetTexturePtr(uint64_t id);
	void RemoveTexture(uint64_t id);

	uint64_t CreateGenericMesh();
	uint64_t CreateGenericMesh(const std::string& name);
	OpenGLGenericMesh& GetGenericMesh(const std::string& name);
	OpenGLGenericMesh& GetGenericMesh(uint64_t id);
	OpenGLGenericMesh* GetGenericMeshPtr(const std::string& name);
	OpenGLGenericMesh* GetGenericMeshPtrById(uint64_t id);
	void RemoveGenericMesh(uint64_t id);

	/*std::string GetResourceInfo();*/
}