#pragma once
#include <Graphics/Primitives/GPFramebuffer.h>

namespace Vortex
{
	class DX11Framebuffer : public GPFramebuffer
	{
	public:
		DX11Framebuffer(GPTexture* texture);
		DX11Framebuffer(IWindow* window);
		virtual ~DX11Framebuffer();

		virtual void Bind() override;
		virtual void Recreate() override;

		virtual void Resize() override;
		virtual void Resize(int width, int height) override;

		virtual void Clear(float r, float g, float b, float a) override;

	private:
		void Create(GPTexture* texture);

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> p_RenderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> p_DepthStencil;
		D3D11_VIEWPORT m_Viewport;

		GPTexture* m_Texture = nullptr;
		IWindow* m_Window = nullptr;
	};
}
