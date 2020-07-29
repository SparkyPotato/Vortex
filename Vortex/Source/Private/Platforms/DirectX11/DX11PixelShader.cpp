#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11PixelShader.h>

namespace Vortex
{
	DX11PixelShader::DX11PixelShader(std::string file)
		: m_File(file)
	{
		IGraphicsContext::Get()->RegisterPrimitive(this);

		Create(m_File);
	}

	DX11PixelShader::~DX11PixelShader()
	{
		IGraphicsContext::Get()->UnregisterPrimitive(this);

		m_Blob->Release();
		m_Shader->Release();
	}

	void DX11PixelShader::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		context->GetContext()->PSSetShader(m_Shader, NULL, 0);
	}

	void DX11PixelShader::Recreate()
	{
		m_Blob->Release();
		m_Shader->Release();

		Create(m_File);
	}

	void DX11PixelShader::Create(std::string file)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		std::string temp = file;
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), NULL, 0);
		std::wstring wfile(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), &wfile[0], size_needed);

		HRESULT hr = D3DCompileFromFile(wfile.c_str(), NULL, NULL, "main", "ps_5_0", NULL, NULL, &m_Blob, NULL);
		if (FAILED(hr))
		{
			throw std::exception("Failed to compile shader!");
		}

		context->GetDevice()->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, &m_Shader);
	}
}
