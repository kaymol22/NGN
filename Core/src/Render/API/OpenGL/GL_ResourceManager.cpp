#include "GL_ResourceManager.h"

namespace OpenGL::ResourceManager
{
	namespace {
		// Global Resource Pools
		NGN::SlotMap<OpenGLShader> g_Shaders;
		NGN::SlotMap<OpenGLFrameBuffer> g_FrameBuffers;
		NGN::SlotMap<OpenGLTexture> g_Textures;
		NGN::SlotMap<OpenGLSSBO> g_SSBOs;
		NGN::SlotMap<OpenGLGenericMesh> g_GenericMeshes;

		// Name -> ID lookup tables
		std::unordered_map<std::string, uint64_t> g_ShaderIdByName;
		std::unordered_map<std::string, uint64_t> g_FrameBufferIdByName;
		std::unordered_map<std::string, uint64_t> g_SSBOIdByName;
		std::unordered_map<std::string, uint64_t> g_GenericMeshIdByName;

		// Id counter for GPU resources
		// Layout : [type(16 bits) | local_id(48 bits)]
		static constexpr uint64_t kTypeShift = 48;
		static constexpr uint64_t kLocalMask = (1ULL << kTypeShift) - 1;

		uint64_t g_NextShaderId = 0;
		uint64_t g_NextFrameBufferId = 0;
		uint64_t g_NextTextureId = 0;
		uint64_t g_NextSSBOId = 0;
		uint64_t g_NextGenericMeshId = 0;

		bool g_Initialized = false;

		uint64_t EncodeId(ResourceType type, uint64_t local) {
			return (uint64_t(static_cast<uint16_t>(type)) << kTypeShift | (local & kLocalMask));
		}

		uint64_t GetNextId(ResourceType type)
		{
			switch (type) {
			case ResourceType::Shader:
				return EncodeId(type, g_NextShaderId++);
			case ResourceType::FrameBuffer:
				return EncodeId(type, g_NextFrameBufferId++);
			case ResourceType::Texture:
				return EncodeId(type, g_NextTextureId++);
			case ResourceType::SSBO:
				return EncodeId(type, g_NextSSBOId++);
			case ResourceType::GenericMesh:
				return EncodeId(type, g_NextGenericMeshId++);
			default: 
				NGN_CORE_ERROR("Unknown resource type");
				return 0;
			}
		}
	}

	void Init()
	{
		if (g_Initialized) {
			NGN_CORE_WARN("OpenGL::ResourceManager::Init() called multiple times");
			return;
		}
		g_Initialized = true;
		NGN_CORE_INFO("OpenGL::ResourceManager Initialized");
	}

	void CleanUp()
	{
		if (!g_Initialized) return;

		for (OpenGLFrameBuffer& frameBuffer : g_FrameBuffers) {
			frameBuffer.CleanUp();
		}
		for (OpenGLTexture& texture : g_Textures) {
			texture.MakeBindlessTextureNonResident();
			texture.Reset();
		}
		for (OpenGLSSBO& ssbo : g_SSBOs) {
			ssbo.CleanUp();
		}
		for (OpenGLGenericMesh mesh : g_GenericMeshes) {
			mesh.CleanUp();
		}

		g_Shaders.clear();
		g_ShaderIdByName.clear();
		g_FrameBuffers.clear();
		g_FrameBufferIdByName.clear();
		g_Textures.clear();
		g_SSBOs.clear();
		g_SSBOIdByName.clear();
		g_GenericMeshes.clear();
		g_GenericMeshIdByName.clear();

		g_Initialized = false;
		NGN_CORE_INFO("OpenGL::ResourceManager shutdown");
	}
	/*##################==================================== SHADER ======================================##################*/
	uint64_t CreateShader(const std::string& name) {
		auto it = g_ShaderIdByName.find(name);
		if (it != g_ShaderIdByName.end()) {
			return it->second;
		}

		uint64_t id = GetNextId(ResourceType::Shader);
		g_Shaders.emplace_with_id(id);
		g_ShaderIdByName[name] = id;
		return id;
	}

	OpenGLShader& LoadShader(const std::string& name, const std::vector<std::string>& paths, const std::vector<std::string>& defines) {
		if (OpenGLShader* shader = GetShaderPtr(name)) {
			NGN_CORE_ERROR("OpenGL::ResourceManager::LoadShader() failed: ", name, " already exists");
			return *shader;
		}

		uint64_t id = CreateShader(name);
		OpenGLShader& shader = GetShaderById(id);
		shader = OpenGLShader(paths, "", defines);
		return shader;
	}

	OpenGLShader& LoadShader(const std::string& subDir, const std::string& name, const std::vector<std::string>& paths, const std::vector<std::string>& defines) {
		if (OpenGLShader* shader = GetShaderPtr(name)) {
			NGN_CORE_ERROR("OpenGL::ResourceManager::LoadShader() failed: ", name, " already exists");
			return *shader;
		}

		uint64_t id = CreateShader(name);
		OpenGLShader& shader = GetShaderById(id);
		shader = OpenGLShader(paths, subDir, defines);
		return shader;
	}

	OpenGLShader& GetShader(const std::string& name) {
		OpenGLShader* shader = GetShaderPtr(name);
		if (shader) {
			return *shader;
		}
		static OpenGLShader invalid;
		return invalid;
	}

	OpenGLShader* GetShaderPtr(const std::string& name) {
		auto it = g_ShaderIdByName.find(name);
		if (it != g_ShaderIdByName.end()) {
			return GetShaderPtrById(it->second);
		}
		return nullptr;
	}

	OpenGLShader& GetShaderById(uint64_t id) {
		OpenGLShader* shader = GetShaderPtrById(id);
		if (shader) {
			return *shader;
		}
		static OpenGLShader invalid;
		return invalid;
	}

	OpenGLShader* GetShaderPtrById(uint64_t id) {
		return g_Shaders.get(id);
	}

	void HotloadShaders() {
		std::string failed = "FAILED TO HOTLOAD";

		bool allSucceeded = true;
		for (OpenGLShader& shader : g_Shaders) {
			if (!shader.HotLoad()) {
				allSucceeded = false;
				failed += "\n- ";
				for (const std::string& path : shader.GetPaths()) {
					failed += path + " ";
				}
			}
		}
		if (allSucceeded) {
			NGN_CORE_INFO("Hotloaded all shaders successfully");
		}
		else {
			NGN_CORE_INFO(failed);
		}
	}

	void RemoveShader(uint64_t id) {
		for (auto it = g_ShaderIdByName.begin(); it != g_ShaderIdByName.end(); ++it) {
			if (it->second == id) {
				g_ShaderIdByName.erase(it);
				break;
			}
		}
		if (g_Shaders.contains(id)) {
			g_Shaders.erase(id);
		}
	}
	/*##################==================================== FRAMEBUFFER ======================================##################*/
	OpenGLFrameBuffer& CreateFrameBuffer(const std::string& name) {
		auto it = g_FrameBufferIdByName.find(name);
		if (it != g_FrameBufferIdByName.end()) {
			return GetFrameBufferById(it->second);
		}

		// Register new Id if not found
		uint64_t id = GetNextId(ResourceType::FrameBuffer);
		g_FrameBuffers.emplace_with_id(id);
		g_FrameBufferIdByName[name] = id;

		OpenGLFrameBuffer& fb = GetFrameBufferById(id);
		fb.SetName(name);
		return fb;
	}

	OpenGLFrameBuffer& GetFrameBuffer(const std::string& name) {
		OpenGLFrameBuffer* fb = GetFrameBufferPtr(name);
		if (fb) {
			return *fb;
		}
		static OpenGLFrameBuffer invalid;
		return invalid;
	}

	OpenGLFrameBuffer* GetFrameBufferPtr(const std::string& name) {
		auto it = g_FrameBufferIdByName.find(name);
		if (it != g_FrameBufferIdByName.end()) {
			return GetFrameBufferPtrById(it->second);
		}
		return nullptr;
	}

	OpenGLFrameBuffer& GetFrameBufferById(uint64_t id) {
		OpenGLFrameBuffer* fb = GetFrameBufferPtrById(id);
		if (fb) {
			return *fb;
		}
		static OpenGLFrameBuffer invalid;
		return invalid;
	}

	OpenGLFrameBuffer* GetFrameBufferPtrById(uint64_t id) {
		return g_FrameBuffers.get(id);
	}

	void RemoveFrameBuffer(uint64_t id) {
		for (auto it = g_FrameBufferIdByName.begin(); it != g_FrameBufferIdByName.end(); ++it) {
			if (it->second == id) {
				g_FrameBufferIdByName.erase(it);
				break;
			}
		}

		if (g_FrameBuffers.contains(id)) {
			g_FrameBuffers.get(id)->CleanUp();
			g_FrameBuffers.erase(id);
		}
	}
	/*##################==================================== SSBO ======================================##################*/
	OpenGLSSBO& CreateSSBO(const std::string& name) {
		auto it = g_SSBOIdByName.find(name);
		if (it != g_SSBOIdByName.end()) {
			return GetSSBOById(it->second);
		}

		uint64_t id = GetNextId(ResourceType::SSBO);
		g_SSBOs.emplace_with_id(id);
		g_SSBOIdByName[name] = id;
		return GetSSBOById(id);
	}

	OpenGLSSBO& GetSSBO(const std::string& name) {
		OpenGLSSBO* ssbo = GetSSBOPtr(name);
		if (ssbo) {
			return *ssbo;
		}
		static OpenGLSSBO invalid;
		return invalid;
	}

	OpenGLSSBO* GetSSBOPtr(const std::string& name) {
		auto it = g_SSBOIdByName.find(name);
		if (it != g_SSBOIdByName.end()) {
			return GetSSBOPtrById(it->second);
		}
		return nullptr;
	}

	OpenGLSSBO& GetSSBOById(uint64_t id) {
		OpenGLSSBO* ssbo = GetSSBOPtrById(id);
		if (ssbo) {
			return *ssbo;
		}
		static OpenGLSSBO invalid;
		return invalid;
	}

	OpenGLSSBO* GetSSBOPtrById(uint64_t id) {
		return g_SSBOs.get(id);
	}

	void RemoveSSBO(uint64_t id) {
		for (auto it = g_SSBOIdByName.begin(); it != g_SSBOIdByName.end(); ++it) {
			if (it->second == id) {
				g_SSBOIdByName.erase(it);
				break;
			}
		}
		if (g_SSBOs.contains(id)) {
			g_SSBOs.get(id)->CleanUp();
			g_SSBOs.erase(id);
		}
	}

	void RemoveSSBOByName(const std::string& name) {
		auto it = g_SSBOIdByName.find(name);
		if (it == g_SSBOIdByName.end()) return;

		RemoveSSBO(it->second);
	}

	/*##################==================================== TEXTURE ======================================##################*/
	uint64_t CreateTexture() {
		uint64_t id = GetNextId(ResourceType::Texture);
		g_Textures.emplace_with_id(id);
		return id;
	}

	OpenGLTexture& GetTexture(uint64_t id) {
		OpenGLTexture* texture = GetTexturePtr(id);
		if (texture) {
			return *texture;
		}
		static OpenGLTexture invalid;
		return invalid;
	}

	OpenGLTexture* GetTexturePtr(uint64_t id) {
		return g_Textures.get(id);
	}

	void RemoveTexture(uint64_t id) {
		if (g_Textures.contains(id)) {
			OpenGLTexture* texture = g_Textures.get(id);
			texture->MakeBindlessTextureNonResident();
			texture->Reset();
			g_Textures.erase(id);
		}
	}
	/*##################==================================== GENERICMESH ======================================##################*/
	uint64_t CreateGenericMesh() {
		uint64_t id = GetNextId(ResourceType::GenericMesh);
		g_GenericMeshes.emplace_with_id(id);
		return id;
	}

	uint64_t CreateGenericMesh(const std::string& name) {
		if (name.empty() || name == UNDEFINED_STRING) {
			return CreateGenericMesh();
		}

		auto it = g_GenericMeshIdByName.find(name);
		if (it != g_GenericMeshIdByName.end()) {
			return it->second;
		}

		uint64_t id = CreateGenericMesh();
		g_GenericMeshIdByName[name] = id;
		return id;
	}

	OpenGLGenericMesh& GetGenericMesh(const std::string& name) {
		OpenGLGenericMesh* mesh = GetGenericMeshPtr(name);
		if (mesh) {
			return *mesh;
		}
		static OpenGLGenericMesh invalid;
		return invalid;
	}

	OpenGLGenericMesh& GetGenericMesh(uint64_t id) {
		OpenGLGenericMesh* mesh = GetGenericMeshPtrById(id);
		if (mesh) {
			return *mesh;
		}
		static OpenGLGenericMesh invalid;
		return invalid;
	}

	OpenGLGenericMesh* GetGenericMeshPtr(const std::string& name) {
		auto it = g_GenericMeshIdByName.find(name);
		if (it != g_GenericMeshIdByName.end()) {
			return GetGenericMeshPtrById(it->second);
		}
		return nullptr;
	}

	OpenGLGenericMesh* GetGenericMeshPtrById(uint64_t id) {
		return g_GenericMeshes.get(id);
	}

	void RemoveGenericMesh(uint64_t id) {
		for (auto it = g_GenericMeshIdByName.begin(); it != g_GenericMeshIdByName.end(); ++it) {
			if (it->second == id) {
				g_GenericMeshIdByName.erase(it);
				break;
			}
		}
		if (g_GenericMeshes.contains(id)) {
			g_GenericMeshes.get(id)->CleanUp();
			g_GenericMeshes.erase(id);
		}
	}
}