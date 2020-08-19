#pragma once
#ifdef PLATFORM_WINDOWS
#include <Graphics/GraphicsContext.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wincodec.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "windowscodecs.lib")

namespace Vortex
{
	class DX11VertexBuffer;
	class DX11VertexShader;

	class DX11GraphicsContext : public GraphicsContext
	{
	public:
		DX11GraphicsContext();
		virtual ~DX11GraphicsContext();

		virtual void Draw(int drawSize) override;

		virtual void Lock() override;
		virtual void Unlock() override;

		ID3D11Device* GetDevice() { return p_Device; }
		ID3D11DeviceContext* GetContext() { return p_Context; }
		IDXGIFactory1* GetFactory() { return p_Factory; }

		void PrintDebugInfo();

		void SetVertexBuffer(DX11VertexBuffer* buffer);
		void SetVertexShader(DX11VertexShader* shader);

		DX11VertexBuffer* GetVertexBuffer() { return m_CurrentVertexBuffer; }
		DX11VertexShader* GetVertexShader() { return m_CurrentVertexShader; }

		void GenerateInputLayout();

	private:
		ID3D11Device* p_Device = nullptr;
		ID3D11DeviceContext* p_Context = nullptr;
		IDXGIFactory1* p_Factory = nullptr;

		std::mutex m_ContextMutex;

		DX11VertexBuffer* m_CurrentVertexBuffer = nullptr;
		DX11VertexShader* m_CurrentVertexShader = nullptr;
	};
}

#endif
