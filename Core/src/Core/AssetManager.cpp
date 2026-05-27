#include "ngnpch.h"
#include "AssetManager.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Mesh.h"
#include "Renderer/SubTexture.h"

namespace NGN
{
    AssetManager* AssetManager::s_Instance = nullptr;

    AssetManager::AssetManager()
    {
        NGN_CORE_ASSERT(!s_Instance, "AssetManager already exists!");
        s_Instance = this;
    }

    AssetManager::~AssetManager()
    {
        s_Instance = nullptr;
    }

    void AssetManager::Init()
    {
        NGN_PROFILE_FUNCTION();
        LoadDefaultAssets();
        SetupFileWatcher();
        NGN_CORE_INFO("AssetManager initialized");
    }

    void AssetManager::Shutdown()
    {
        NGN_PROFILE_FUNCTION();
        ClearCache();
        m_DefaultShader = nullptr;
        m_WhiteTexture = nullptr;
        m_ErrorTexture = nullptr;
        NGN_CORE_INFO("AssetManager shutdown");
    }

    void AssetManager::OnUpdate()
    {
        NGN_PROFILE_FUNCTION();
        CheckForModifiedShaders();
    }

    // ==================== Shader Management ====================

    Ref<Shader> AssetManager::GetShader(const std::filesystem::path& filePath)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = filePath.string();

        // Check cache first
        if (m_ShaderCache.contains(key))
        {
            return m_ShaderCache[key];
        }

        // Load shader
        auto shader = Shader::Create(filePath);
        if (!shader)
        {
            NGN_CORE_ERROR("Failed to load shader: {}", key);
            return m_DefaultShader;
        }

        // Cache it
        m_ShaderCache[key] = shader;

        // Setup file watching for hot-reload
        if (std::filesystem::exists(filePath))
        {
            ShaderFileInfo& fileInfo = m_ShaderFileInfo[key];
            fileInfo.FilePath = filePath;
            fileInfo.LastModifiedTime = std::filesystem::last_write_time(filePath);
        }

        NGN_CORE_TRACE("Shader cached: {}", key);
        return shader;
    }

    Ref<Shader> AssetManager::GetShader(const std::string& name,
                                        const std::filesystem::path& vertexPath,
                                        const std::filesystem::path& fragmentPath)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = name + ":" + vertexPath.string() + ":" + fragmentPath.string();

        if (m_ShaderCache.contains(key))
        {
            return m_ShaderCache[key];
        }

        auto shader = Shader::Create(name, vertexPath, fragmentPath);
        if (!shader)
        {
            NGN_CORE_ERROR("Failed to load shader: {} (vertex: {}, fragment: {})", name, vertexPath.string(), fragmentPath.string());
            return m_DefaultShader;
        }

            m_ShaderCache[key] = shader;

            NGN_CORE_TRACE("Shader cached: {}", key);
            return shader;
        }

    Ref<Shader> AssetManager::GetDefaultShader()
    {
        return m_DefaultShader;
    }

    void AssetManager::RegisterShaderReloadCallback(const std::filesystem::path& filePath, ShaderReloadCallback callback)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = filePath.string();

        // Make sure exists before accessing
        if (m_ShaderFileInfo.contains(key) && std::filesystem::exists(filePath))
        {
            ShaderFileInfo& fileInfo = m_ShaderFileInfo[key];
            fileInfo.FilePath = filePath;
        }

        // Create callback for new or existing entry
        if (m_ShaderFileInfo.contains(key))
        {
            m_ShaderFileInfo[key].ReloadCallbacks.push_back(callback);
            NGN_CORE_TRACE("Registered reload callback for shader: {}", key);
        }
        else
        {
            NGN_CORE_WARN("Can't register reload callback - shader file not found: {}", key);
        }
    }

    // ==================== Texture Management ====================

    Ref<Texture2D> AssetManager::GetTexture(const std::filesystem::path& filePath)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = filePath.string();

        if (m_TextureCache.contains(key))
        {
            return m_TextureCache[key];
        }

        auto texture = Texture2D::Create(filePath.string());
        if (!texture)
        {
            NGN_CORE_ERROR("Failed to load texture: {}", key);
            return m_ErrorTexture;
        }

        m_TextureCache[key] = texture;
        NGN_CORE_TRACE("Texture cached: {}", key);
        return texture;
    }

    Ref<Texture2D> AssetManager::GetTexture(const TextureSpecification& spec)
    {
        NGN_PROFILE_FUNCTION();

        // Create a key from the specification
        std::string key = std::format("spec_{}x{}_{}", spec.Width, spec.Height, static_cast<int>(spec.Format));

        if (m_TextureCache.contains(key))
        {
            return m_TextureCache[key];
        }

        auto texture = Texture2D::Create(spec);
        m_TextureCache[key] = texture;
        NGN_CORE_TRACE("Texture created from spec: {}", key);
        return texture;
    }

    Ref<Texture2D> AssetManager::GetWhiteTexture()
    {
        return m_WhiteTexture;
    }

    Ref<Texture2D> AssetManager::GetErrorTexture()
    {
        return m_ErrorTexture;
    }

    // ==================== Mesh Management ====================

    Ref<Mesh> AssetManager::GetMesh(const std::filesystem::path& filePath)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = filePath.string();

        if (m_MeshCache.contains(key))
        {
            return m_MeshCache[key];
        }

        // TODO: Implement mesh file loading (OBJ, glTF, etc.)
        NGN_CORE_WARN("Mesh file loading not yet implemented: {}", key);
        return nullptr;
    }

    Ref<Mesh> AssetManager::GetCube(float size)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = std::format("cube_{}", size);

        if (m_MeshCache.contains(key))
        {
            return m_MeshCache[key];
        }

        auto mesh = Mesh::CreateCube(size);
        m_MeshCache[key] = mesh;
        NGN_CORE_TRACE("Cube mesh cached: {}", key);
        return mesh;
    }

    Ref<Mesh> AssetManager::GetPyramid(float size)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = std::format("pyramid_{}", size);

        if (m_MeshCache.contains(key))
        {
            return m_MeshCache[key];
        }

        auto mesh = Mesh::CreatePyramid(size);
        m_MeshCache[key] = mesh;
        NGN_CORE_TRACE("Pyramid mesh cached: {}", key);
        return mesh;
    }

    Ref<Mesh> AssetManager::GetPlane(float width, float height)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = std::format("plane_{}x{}", width, height);

        if (m_MeshCache.contains(key))
        {
            return m_MeshCache[key];
        }

        auto mesh = Mesh::CreatePlane(width, height);
        m_MeshCache[key] = mesh;
        NGN_CORE_TRACE("Plane mesh cached: {}", key);
        return mesh;
    }

    Ref<Mesh> AssetManager::GetSphere(float radius, uint32_t segments)
    {
        NGN_PROFILE_FUNCTION();

        std::string key = std::format("sphere_{}_{}", radius, segments);

        if (m_MeshCache.contains(key))
        {
            return m_MeshCache[key];
        }

        // TODO: Implement sphere creation
        NGN_CORE_WARN("Sphere mesh not yet implemented");
        return nullptr;
    }

    // ==================== SubTexture Management ====================

    Ref<SubTexture2D> AssetManager::GetSubTexture(const Ref<Texture2D>& texture,
                                                   const glm::vec2& coords,
                                                   const glm::vec2& cellSize)
    {
        NGN_PROFILE_FUNCTION();

        if (!texture)
            return nullptr;

        // Create a unique key for this subtexture
        // Convert glm::vec2 to a simple string representation instead of hashing
        std::string key = std::format("subtex_{}_({},{})_({},{})",
            texture->GetRendererID(),
            coords.x, coords.y,
            cellSize.x, cellSize.y
        );

        if (m_SubTextureCache.contains(key))
        {
            return m_SubTextureCache[key];
        }

        auto subTexture = SubTexture2D::CreateFromCoords(texture, coords, { 1, 1 }, cellSize);
        m_SubTextureCache[key] = subTexture;
        NGN_CORE_TRACE("SubTexture cached: {}", key);
        return subTexture;
    }

    // ==================== Cache Management ====================

    void AssetManager::ClearCache()
    {
        NGN_PROFILE_FUNCTION();
        ClearShaderCache();
        ClearTextureCache();
        ClearMeshCache();
    }

    void AssetManager::ClearShaderCache()
    {
        NGN_PROFILE_FUNCTION();
        m_ShaderCache.clear();
        m_ShaderFileInfo.clear();
        NGN_CORE_INFO("Shader cache cleared");
    }

    void AssetManager::ClearTextureCache()
    {
        NGN_PROFILE_FUNCTION();
        m_TextureCache.clear();
        NGN_CORE_INFO("Texture cache cleared");
    }

    void AssetManager::ClearMeshCache()
    {
        NGN_PROFILE_FUNCTION();
        m_MeshCache.clear();
        NGN_CORE_INFO("Mesh cache cleared");
    }

    AssetManager::CacheStats AssetManager::GetCacheStats() const
    {
        return {
            m_ShaderCache.size(),
            m_TextureCache.size(),
            m_MeshCache.size()
        };
    }

    AssetManager& AssetManager::Get()
    {
        NGN_CORE_ASSERT(s_Instance, "AssetManager not initialized!");
        return *s_Instance;
    }

    // ==================== Private Methods ====================

    void AssetManager::LoadDefaultAssets()
    {
        NGN_PROFILE_FUNCTION();

        // Create a simple default shader (white texture pass-through)
        // This will be used as fallback when shader loading fails
        m_DefaultShader = Shader::Create("assets/Shaders/Default.glsl");
        if (!m_DefaultShader)
        {
            NGN_CORE_WARN("Failed to load default shader - shaders may fail gracefully");
        }

        // Create white texture for debugging (1x1 white pixel)
        TextureSpecification whiteSpec;
        whiteSpec.Width = 1;
        whiteSpec.Height = 1;
        whiteSpec.Format = ImageFormat::RGBA8;
        m_WhiteTexture = Texture2D::Create(whiteSpec);
        if (m_WhiteTexture)
        {
            uint32_t whiteData = 0xFFFFFFFF;
            m_WhiteTexture->SetData(&whiteData, sizeof(uint32_t));
        }

        // Create error texture (magenta - 1x1 pixel)
        // This helps identify missing textures in debug
        TextureSpecification errorSpec;
        errorSpec.Width = 1;
        errorSpec.Height = 1;
        errorSpec.Format = ImageFormat::RGBA8;
        m_ErrorTexture = Texture2D::Create(errorSpec);
        if (m_ErrorTexture)
        {
            uint32_t magentaData = 0xFF00FFFF;  // Magenta in RGBA
            m_ErrorTexture->SetData(&magentaData, sizeof(uint32_t));
        }

        NGN_CORE_INFO("Default assets loaded");
    }

    void AssetManager::SetupFileWatcher()
    {
        NGN_PROFILE_FUNCTION();
        // File watcher will be checked in OnUpdate
        NGN_CORE_TRACE("File watcher setup complete");
    }

    void AssetManager::CheckForModifiedShaders()
    {
        NGN_PROFILE_FUNCTION();
    }
}
