#pragma once

#include "GL_Utils.h"
#include "Types/GL_Texture.h"
#include "ResourceManagement/CPU/Types/Texture.h"

namespace OpenGL::TextureUploader
{
	bool ValidateTexture(RS::Texture& texture);
	bool UploadToGPUTexture(RS::Texture& texture, OpenGLTexture& gpuTexture);
}