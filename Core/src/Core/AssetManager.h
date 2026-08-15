#pragma once

#include "Base.h"
#include "Renderer/Resources/Shader.h"
#include "Renderer/Resources/Texture.h"
#include "Renderer/Resources/Mesh.h"
#include "Renderer/Resources/SubTexture.h"

#include <filesystem>
#include <unordered_map>
#include <functional>
#include <chrono>

namespace NGN
{
    // Asset manager handles:
    // - Caching and reuse of assets (shaders, textures, meshes)
    // - Hot-reload of shaders (with file watching)
    // - Default fallback assets for error handling
    // - Centralized lifecycle management

    class AssetManager
    {
    public:
        AssetManager();
        ~AssetManager();

        // Sets up file watchers*
        void Init();
        void Shutdown();

        // Update - checks for file changes and triggers hot-reloads
        void OnUpdate();

        // ==================== Shader Management ====================

        // Get or load a shader (cached)
        Ref<Shader> GetShader(const std::filesystem::path& filePath);

        // Get or load shader from vertex and fragment sources
        Ref<Shader> GetShader(const std::string& name, 
                              const std::filesystem::path& vertexPath,
                              const std::filesystem::path& fragmentPath);

        // Get default/fallback shader (used when shader loading fails)
        Ref<Shader> GetDefaultShader();

        // Register a reload callback for a shader (called when shader reloads)
        using ShaderReloadCallback = std::function<void(Ref<Shader>)>;
        void RegisterShaderReloadCallback(const std::filesystem::path& filePath, ShaderReloadCallback callback);

        // ==================== Texture Management ====================

        // Get or load a texture (cached)
        Ref<Texture2D> GetTexture(const std::filesystem::path& filePath);

        // Create texture from specification
        Ref<Texture2D> GetTexture(const TextureSpecification& spec);

        // Get default white texture (for debugging/fallback)
        Ref<Texture2D> GetWhiteTexture();

        // Get default error texture (magenta - indicates missing asset)
        Ref<Texture2D> GetErrorTexture();

        // ==================== Mesh Management ====================

        // Get or load mesh (cached)
        Ref<Mesh> GetMesh(const std::filesystem::path& filePath);

        // Get parametric meshes (also cached)
        Ref<Mesh> GetCube(float size = 1.0f);
        Ref<Mesh> GetPyramid(float size = 1.0f);
        Ref<Mesh> GetPlane(float width = 1.0f, float height = 1.0f);

        // Get default sphere mesh
        Ref<Mesh> GetSphere(float radius = 1.0f, uint32_t segments = 32);

        // ==================== SubTexture Management ====================

        // Get or create subtexture
        Ref<SubTexture2D> GetSubTexture(const Ref<Texture2D>& texture,
                                        const glm::vec2& coords,
                                        const glm::vec2& cellSize);

        // ==================== Cache Management ====================

        // Clear all cached assets (careful - breaks references!)
        void ClearCache();

        // Clear specific asset category
        void ClearShaderCache();
        void ClearTextureCache();
        void ClearMeshCache();

        // Get cache statistics
        struct CacheStats
        {
            size_t ShaderCount;
            size_t TextureCount;
            size_t MeshCount;
        };
        CacheStats GetCacheStats() const;

        // ==================== Static Interface ====================
        // Singleton access
        static AssetManager& Get();

    private:
        // Internal methods
        void LoadDefaultAssets();
        void SetupFileWatcher();
        void CheckForModifiedShaders();

        // File modification tracking
        struct ShaderFileInfo
        {
            std::filesystem::path FilePath;
            std::chrono::file_clock::time_point LastModifiedTime;
            std::vector<ShaderReloadCallback> ReloadCallbacks;

            ShaderFileInfo() = default;
            ShaderFileInfo(const std::filesystem::path& path)
				: FilePath(path), LastModifiedTime(std::filesystem::last_write_time(path)) {}
        };

        // Caches
        std::unordered_map<std::string, Ref<Shader>> m_ShaderCache;
        std::unordered_map<std::string, Ref<Texture2D>> m_TextureCache;
        std::unordered_map<std::string, Ref<Mesh>> m_MeshCache;
        std::unordered_map<std::string, Ref<SubTexture2D>> m_SubTextureCache;

        // File tracking for hot-reload
        std::unordered_map<std::string, ShaderFileInfo> m_ShaderFileInfo;

        // Default assets
        Ref<Shader> m_DefaultShader;
        Ref<Texture2D> m_WhiteTexture;
        Ref<Texture2D> m_ErrorTexture;

        // Singleton
        static AssetManager* s_Instance;
    };
}
