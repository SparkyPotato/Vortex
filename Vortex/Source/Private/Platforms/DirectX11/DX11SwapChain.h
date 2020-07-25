#pragma once
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>
#include <Private/Platforms/Windows/WWindow.h>
#include <Graphics/Primitives/GPSwapChain.h>

namespace Vortex
{
	class DX11SwapChain : public GPSwapChain
	{
	public:
		DX11SwapChain(IWindow* window);
		virtual ~DX11SwapChain();

		virtual void Bind() override;
		virtual void Recreate() override;

		virtual GPTexture* GetBackBuffer() override;

		virtual void Resize() override;
		virtual void Swap(int syncInterval) override;

	private:
		WWindow* m_Window = nullptr;
		IDXGISwapChain* p_SwapChain = nullptr;
	};
}
