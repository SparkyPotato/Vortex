#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11VertexShader.h>

namespace Vortex
{
	DX11VertexShader::DX11VertexShader(std::string file)
		: m_File(file)
	{
		IGraphicsContext::Get()->RegisterPrimitive(this);

		Create(m_File);
	}

	DX11VertexShader::~DX11VertexShader()
	{
		IGraphicsContext::Get()->UnregisterPrimitive(this);

		m_Shader->Release();
		m_Blob->Release();
	}

	void DX11VertexShader::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		context->GetContext()->VSSetShader(m_Shader, NULL, 0);

		context->SetVertexShader(this);
	}

	void DX11VertexShader::Recreate()
	{
		m_Blob->Release();
		m_Shader->Release();

		Create(m_File);
	}

	void DX11VertexShader::Create(std::string file)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());

		std::string temp = file;
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), NULL, 0);
		std::wstring wfile(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), &wfile[0], size_needed);

		HRESULT hr = D3DCompileFromFile(wfile.c_str(), NULL, NULL, "main", "vs_5_0", NULL, NULL, &m_Blob, NULL);
		if (hr == E_FAIL)
		{
			throw std::exception("Failed to compile shader!");
		}

		context->GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, &m_Shader);
	}
}
