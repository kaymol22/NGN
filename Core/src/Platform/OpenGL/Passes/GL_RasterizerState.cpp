#include "GL_RasterizerState.h"

namespace NGN
{
	void OpenGLRasterizerState::Apply() const
	{
		// Blending
		blendEnable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		glBlendFunc(blendSrcFactor, blendDstFactor);

		// Colour Mask
		glColorMask(colorMask, colorMask, colorMask, colorMask);

		// Depth
		depthTestEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		glDepthMask(depthMask);
		glDepthFunc(depthFunc);

		// Culling
		cullFaceEnable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		glCullFace(cullFaceMode);

		// Stencil
		stencilTestEnable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
		glStencilFunc(stencilFunc, stencilRef, stencilReadMask);
		glStencilMask(stencilWriteMask);
		glStencilOp(stencilFailOp, stencilDepthFailOp, stencilPassOp);

		// Misc
		glPointSize(pointSize);
	}
}