#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	enum class TextureUsage
	{
		ShaderResource ,
		Framebuffer,
		ColorTexture
	};

	class GPTexture : public GraphicsPrimitive
	{
	public:
		virtual ~GPTexture() {}

		static GPTexture* Create(int width, int height);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		virtual void Resize(int width, int height) = 0;
	};
}
