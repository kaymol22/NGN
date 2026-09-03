#include "ngnpch.h"

#include "GL_Utils.h"

#include <format>

namespace OpenGL::Utils {

    // These EXT_texture_sRGB/S3TC enums are not exposed by this project's
    // generated GLAD header, despite being valid OpenGL internal formats.
    constexpr GLenum GL_COMPRESSED_SRGB_S3TC_DXT1 = 0x8C4C;
    constexpr GLenum GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1 = 0x8C4D;
    constexpr GLenum GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3 = 0x8C4E;
    constexpr GLenum GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5 = 0x8C4F;

    const char* GLDebugSourceToString(GLenum source)
    {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:					return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:			return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:		return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:			return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:			return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:					return "OTHER";
        default:									return "UNKNOWN";
        }
    }

    const char* GLDebugTypeToString(GLenum type)
    {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:					return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		return "DEPRECATED BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		return "UDEFINED BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: 			return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE:				return "PERFORMANCE";
        case GL_DEBUG_TYPE_OTHER:					return "OTHER";
        case GL_DEBUG_TYPE_MARKER:					return "MARKER";
        default:									return "UNKNOWN";
        }
    }

    const char* GLDebugSeverityToString(GLenum severity)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:				return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:				return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:					return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:		return "NOTIFICATION";
        default:									return "UNKNOWN";
        }
    }

    static void GLDebugCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        // TODO: Custom filters
        if (severity != GL_DEBUG_SEVERITY_MEDIUM && severity != GL_DEBUG_SEVERITY_HIGH)
            return;

        const char* sourceStr = OpenGL::Utils::GLDebugSourceToString(source);
        const char* typeStr = OpenGL::Utils::GLDebugTypeToString(type);
        const char* severityStr = OpenGL::Utils::GLDebugSeverityToString(severity);

        std::cout << std::format("[OpenGL] [{} - {} ({})]: [{}] {}\n", severityStr, typeStr, id, sourceStr, message);
    }

    void InitOpenGLDebugMessageCallback()
    {
        glDebugMessageCallback(GLDebugCallback, nullptr);
    }

    GLint GetFormatFromInternalFormat(GLint internalFormat)
    {
        switch (internalFormat) {
            // R
        case GL_R8:
        case GL_R8_SNORM:
        case GL_R16:
        case GL_R16_SNORM:
        case GL_R16F:
        case GL_R32F:
            return GL_RED;

        case GL_R8UI:
        case GL_R8I:
        case GL_R16UI:
        case GL_R16I:
        case GL_R32UI:
        case GL_R32I:
            return GL_RED_INTEGER;

            // RG
        case GL_RG8:
        case GL_RG8_SNORM:
        case GL_RG16:
        case GL_RG16_SNORM:
        case GL_RG16F:
        case GL_RG32F:
            return GL_RG;

        case GL_RG8UI:
        case GL_RG8I:
        case GL_RG16UI:
        case GL_RG16I:
        case GL_RG32UI:
        case GL_RG32I:
            return GL_RG_INTEGER;

            // RGB
        case GL_RGB8:
        case GL_RGB8_SNORM:
        case GL_RGB16:
        case GL_RGB16_SNORM:
        case GL_RGB16F:
        case GL_RGB32F:
        case GL_SRGB8:
        case GL_R11F_G11F_B10F:
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_SRGB_S3TC_DXT1:
        case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
        case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
            return GL_RGB;

        case GL_RGB8UI:
        case GL_RGB8I:
        case GL_RGB16UI:
        case GL_RGB16I:
        case GL_RGB32UI:
        case GL_RGB32I:
            return GL_RGB_INTEGER;

            // RGBA
        case GL_RGBA8:
        case GL_RGBA8_SNORM:
        case GL_RGBA16:
        case GL_RGBA16_SNORM:
        case GL_RGBA16F:
        case GL_RGBA32F:
        case GL_SRGB8_ALPHA8:
        case GL_RGB10_A2: // packed but uploads as RGBA
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5:
        case GL_COMPRESSED_RGBA_BPTC_UNORM:
        case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
            return GL_RGBA;

        case GL_RGBA8UI:
        case GL_RGBA8I:
        case GL_RGBA16UI:
        case GL_RGBA16I:
        case GL_RGBA32UI:
        case GL_RGBA32I:
        case GL_RGB10_A2UI:
            return GL_RGBA_INTEGER;

            // Compressed one/two-channel formats
        case GL_COMPRESSED_RED_RGTC1:
            return GL_RED;

        case GL_COMPRESSED_RG_RGTC2:
            return GL_RG;

            // Depth / stencil
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
            return GL_DEPTH_COMPONENT;

        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
            return GL_DEPTH_STENCIL;

        default:
            std::cout << "GetFormatFromInternalFormat: unsupported internal format " << internalFormat << "\n";
            return -1;
        }
    }

    GLenum GLInternalFormatToGLType(GLenum internalFormat) {
        switch (internalFormat) {
            // Integers
        case GL_R8UI:  case GL_RG8UI:  case GL_RGBA8UI:    return GL_UNSIGNED_BYTE;
        case GL_R8I:   case GL_RG8I:   case GL_RGBA8I:     return GL_BYTE;
        case GL_R16UI: case GL_RG16UI: case GL_RGBA16UI:   return GL_UNSIGNED_SHORT;
        case GL_R16I:  case GL_RG16I:  case GL_RGBA16I:    return GL_SHORT;
        case GL_R32UI: case GL_RG32UI: case GL_RGBA32UI:   return GL_UNSIGNED_INT;
        case GL_R32I:  case GL_RG32I:  case GL_RGBA32I:    return GL_INT;
        case GL_RGB10_A2UI:                                return GL_UNSIGNED_INT_2_10_10_10_REV;
        case GL_RGB10_A2:                                  return GL_UNSIGNED_INT_2_10_10_10_REV;

            // Normalized
        case GL_R8: case GL_RG8: case GL_RGBA8:
        case GL_SRGB8: case GL_SRGB8_ALPHA8:               return GL_UNSIGNED_BYTE;
        case GL_R16: case GL_RG16: case GL_RGBA16:         return GL_UNSIGNED_SHORT;

            // Floats
        case GL_R16F: case GL_RG16F: case GL_RGBA16F:      return GL_HALF_FLOAT;
        case GL_R32F: case GL_RG32F: case GL_RGBA32F:      return GL_FLOAT;
        case GL_R11F_G11F_B10F:                            return GL_FLOAT;

            // Depth/stencil
        case GL_DEPTH_COMPONENT16:                         return GL_UNSIGNED_SHORT;
        case GL_DEPTH_COMPONENT24:                         return GL_UNSIGNED_INT;
        case GL_DEPTH_COMPONENT32F:                        return GL_FLOAT;
        case GL_DEPTH24_STENCIL8:                          return GL_UNSIGNED_INT_24_8;
        case GL_DEPTH32F_STENCIL8:                         return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

        default:
            std::cout << "GLInternalFormatToGLType(): unsupported type\n";
            return 0;
        }
    }

    GLenum GLInternalFormatToGLFormat(GLenum internalFormat)
    {
        switch (internalFormat) {
            // Red channel formats
        case GL_R8:
        case GL_R8_SNORM:
        case GL_R16:
        case GL_R16_SNORM:
        case GL_R16F:
        case GL_R32F:
            return GL_RED;
        case GL_R8UI:
        case GL_R8I:
        case GL_R16UI:
        case GL_R16I:
        case GL_R32UI:
        case GL_R32I:
            return GL_RED_INTEGER;

            // Red-Green channel formats
        case GL_RG8:
        case GL_RG8_SNORM:
        case GL_RG16:
        case GL_RG16_SNORM:
        case GL_RG16F:
        case GL_RG32F:
            return GL_RG;
        case GL_RG8UI:
        case GL_RG8I:
        case GL_RG16UI:
        case GL_RG16I:
        case GL_RG32UI:
        case GL_RG32I:
            return GL_RG_INTEGER;

            // RGB channel formats
        case GL_RGB8:
        case GL_RGB8_SNORM:
        case GL_RGB16:
        case GL_RGB16_SNORM:
        case GL_RGB16F:
        case GL_RGB32F:
        case GL_SRGB8:
        case GL_R11F_G11F_B10F:
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_SRGB_S3TC_DXT1:
        case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
        case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
            return GL_RGB;
        case GL_RGB8UI:
        case GL_RGB8I:
        case GL_RGB16UI:
        case GL_RGB16I:
        case GL_RGB32UI:
        case GL_RGB32I:
            return GL_RGB_INTEGER;

            // RGBA channel formats
        case GL_RGBA8:
        case GL_RGBA8_SNORM:
        case GL_RGBA16:
        case GL_RGBA16_SNORM:
        case GL_RGBA16F:
        case GL_RGBA32F:
        case GL_SRGB8_ALPHA8:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5:
        case GL_COMPRESSED_RGBA_BPTC_UNORM:
        case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
            return GL_RGBA;
        case GL_RGBA8UI:
        case GL_RGBA8I:
        case GL_RGBA16UI:
        case GL_RGBA16I:
        case GL_RGBA32UI:
        case GL_RGBA32I:
            return GL_RGBA_INTEGER;

            // Special packed formats
        case GL_RGB10_A2:
        case GL_RGB10_A2UI:
            return GL_RGBA;

            // Compressed one/two-channel formats
        case GL_COMPRESSED_RED_RGTC1:
            return GL_RED;
        case GL_COMPRESSED_RG_RGTC2:
            return GL_RG;

            // Depth formats
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
            return GL_DEPTH_COMPONENT;

            // Depth-stencil formats
        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
            return GL_DEPTH_STENCIL;

        default:
            std::cout << "GLInternalFormatToGLFormat: Unsupported internal format " << internalFormat << "\n";
            return 0;
        }
    }

    const char* GLInternalFormatToString(GLenum internalFormat)
    {
        switch (internalFormat) {
        case GL_R8: return "GL_R8";
        case GL_RG8: return "GL_RG8";
        case GL_RGB8: return "GL_RGB8";
        case GL_RGBA8: return "GL_RGBA8";
        case GL_R16F: return "GL_R16F";
        case GL_RG16F: return "GL_RG16F";
        case GL_RGB16F: return "GL_RGB16F";
        case GL_RGBA16F: return "GL_RGBA16F";
        case GL_R11F_G11F_B10F: return "GL_R11F_G11F_B10F";
        default: return "Unknown internal format";
        }
    }

    GLint TextureWrapModeToGLEnum(TextureWrapMode wrapMode)
    {
        switch (wrapMode) {
        case TextureWrapMode::REPEAT: return GL_REPEAT;
        case TextureWrapMode::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
        case TextureWrapMode::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
        case TextureWrapMode::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
        default: return GL_NONE;
        }
    }

    GLint TextureFilterToGLEnum(TextureFilter filter)
    {
        switch (filter) {
        case TextureFilter::NEAREST: return GL_REPEAT;
        case TextureFilter::LINEAR: return GL_LINEAR;
        case TextureFilter::LINEAR_MIPMAP: return GL_LINEAR_MIPMAP_LINEAR;
        default: return GL_NONE;
        }
    }

    size_t CalculateTexture2DByteCount(uint32_t width, uint32_t height, GLenum internalFormat, uint32_t mipmapLevelCount, uint32_t sampleCount)
    {
        if (width == 0 || height == 0 || mipmapLevelCount == 0 || sampleCount == 0) {
            return 0;
        }

        size_t byteCount = 0;
        const size_t bytesPerPixel = GetBytesPerPixel(internalFormat);
        const size_t compressedBlockSize = GetCompressedBlockSize(internalFormat);

        for (uint32_t mipLevel = 0; mipLevel < mipmapLevelCount; mipLevel++)
        {
            const size_t mipWidth = static_cast<size_t>(std::max(1u, width >> mipLevel));
            const size_t mipHeight = static_cast<size_t>(std::max(1u, height >> mipLevel));

            if (compressedBlockSize > 0) {
                const size_t blockCountX = (mipWidth + 3) / 4;
                const size_t blockCountY = (mipWidth + 3) / 4;
                byteCount += blockCountX * blockCountY * compressedBlockSize * sampleCount;
            }
            else {
                byteCount += mipWidth * mipHeight * bytesPerPixel * sampleCount;
            }
        }
        return byteCount;
    }

    size_t GetBytesPerPixel(GLenum internalFormat)
    {
        switch (internalFormat) {
        case GL_R8:
        case GL_R8_SNORM:
        case GL_R8UI:
        case GL_R8I:
            return 1;

        case GL_RG8:
        case GL_RG8_SNORM:
        case GL_RG8UI:
        case GL_RG8I:
        case GL_R16:
        case GL_R16_SNORM:
        case GL_R16F:
        case GL_R16UI:
        case GL_R16I:
        case GL_DEPTH_COMPONENT16:
            return 2;

        case GL_RGB8:
        case GL_RGB8_SNORM:
        case GL_RGB8UI:
        case GL_RGB8I:
        case GL_SRGB8:
        case GL_DEPTH_COMPONENT24:
            return 3;

        case GL_RGBA8:
        case GL_RGBA8_SNORM:
        case GL_RGBA8UI:
        case GL_RGBA8I:
        case GL_SRGB8_ALPHA8:
        case GL_RG16:
        case GL_RG16_SNORM:
        case GL_RG16F:
        case GL_RG16UI:
        case GL_RG16I:
        case GL_R32F:
        case GL_R32UI:
        case GL_R32I:
        case GL_R11F_G11F_B10F:
        case GL_RGB10_A2:
        case GL_RGB10_A2UI:
        case GL_DEPTH24_STENCIL8:
            return 4;

        case GL_DEPTH32F_STENCIL8:
            return 5;

        case GL_RGB16:
        case GL_RGB16_SNORM:
        case GL_RGB16F:
        case GL_RGB16UI:
        case GL_RGB16I:
            return 6;

        case GL_RGBA16:
        case GL_RGBA16_SNORM:
        case GL_RGBA16F:
        case GL_RGBA16UI:
        case GL_RGBA16I:
        case GL_RG32F:
        case GL_RG32UI:
        case GL_RG32I:
            return 8;

        case GL_RGB32F:
        case GL_RGB32UI:
        case GL_RGB32I:
            return 12;

        case GL_RGBA32F:
        case GL_RGBA32UI:
        case GL_RGBA32I:
            return 16;

        case GL_DEPTH_COMPONENT32F:
            return 4;

        default:
            return 0;
        }
    }

    size_t GetCompressedBlockSize(GLenum internalFormat)
    {
        switch (internalFormat) {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_SRGB_S3TC_DXT1:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1:
        case GL_COMPRESSED_RED_RGTC1:
            return 8;

        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5:
        case GL_COMPRESSED_RG_RGTC2:
        case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:
        case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:
        case GL_COMPRESSED_RGBA_BPTC_UNORM:
        case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
            return 16;

        default:
            return 0;
        }
    }
}