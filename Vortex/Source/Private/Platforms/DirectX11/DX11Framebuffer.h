#pragma once
#include <Graphics/Primitives/GPFramebuffer.h>
#include <Private/Platforms/DirectX11/DX11Texture.h>

namespace Vortex
{
	class DX11Framebuffer : public GPFramebuffer
	{
	public:
		DX11Framebuffer(GPTexture* texture);
		DX11Framebuffer(Window* window);
		virtual ~DX11Framebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Recreate() override;

		virtual void Resize() override;
		virtual void Resize(int width, int height) override;

		virtual void Clear(float r, float g, float b, float a) override;

		virtual GPTexture* GetTexture() override { return m_Texture; }

		virtual void DisableDepth() override;

	private:
		void Create(DX11Texture* texture);

		ID3D11RenderTargetView* p_RenderTarget;
		ID3D11DepthStencilView* p_DepthStencil;
		D3D11_VIEWPORT m_Viewport;

		DX11Texture* m_Texture = nullptr;
		Window* m_Window = nullptr;
	};
}
