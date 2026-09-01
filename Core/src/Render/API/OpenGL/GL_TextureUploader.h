#pragma once

#include "GL_Utils.h"
#include "Types/GL_Texture.h"
#include "ResourceManagement/CPU/Types/Texture.h"

namespace OpenGL::TextureUploader
{
	bool ValidateTexture(RS::Texture& texture);
	OpenGLTexture& CreatGPUTexture(RS::Texture& texture);
}