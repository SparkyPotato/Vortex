#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	/*
		Where is the texture going to be used?
	*/
	enum class TextureUsage
	{
		ShaderResource ,
		Framebuffer,
		ColorTexture // This is just a combination of ShaderResource and Framebuffer.
	};

	/*
		A texture. Pretty self-explanatory.
	*/
	class GPTexture : public GraphicsPrimitive
	{
	public:
		virtual ~GPTexture() {}

		/*
			Creates a texture from the specified width and height.
		*/
		static GPTexture* Create(int width, int height);
		/*
			Creates a texture for a particular usage from the specified width and height.
		*/
		static GPTexture* Create(int width, int height, TextureUsage usage);

		/*
			Binding a texture does nothing.
			It will probably bind it to the current Pixel Shader in the future.
		*/
		virtual void Bind() = 0;
		/*
			Recreates the texuture.
		*/
		virtual void Recreate() = 0;

		/*
			Returns the current width of the texture.
		*/
		virtual int GetWidth() = 0;
		/*
			Returns the current height of the texture.
		*/
		virtual int GetHeight() = 0;

		/*
			Resizes the texture to the specified width and height.
		*/
		virtual void Resize(int width, int height) = 0;
	};
}
