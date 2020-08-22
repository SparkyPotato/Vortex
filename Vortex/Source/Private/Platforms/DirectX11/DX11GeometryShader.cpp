#include <VXpch.h>
#include <Private/Platforms/DirectX11/DX11GeometryShader.h>

namespace Vortex
{
	DX11GeometryShader::DX11GeometryShader(std::string file, std::string defines)
		: m_File(file), m_Defines(defines)
	{
		GraphicsContext::Get()->RegisterPrimitive(this);

		Create(m_File, m_Defines);
	}

	DX11GeometryShader::~DX11GeometryShader()
	{
		GraphicsContext::Get()->UnregisterPrimitive(this);

		m_Shader->Release();
		m_Blob->Release();
	}

	void DX11GeometryShader::Bind()
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		context->Lock();
		context->GetContext()->GSSetShader(m_Shader, NULL, 0);
		context->Unlock();
	}

	void DX11GeometryShader::Recreate()
	{
		m_Blob->Release();
		m_Shader->Release();

		Create(m_File, m_Defines);
	}

	void DX11GeometryShader::Create(std::string file, std::string defines)
	{
		DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());

		std::string temp = file;
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), NULL, 0);
		std::wstring wfile(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), &wfile[0], size_needed);

		UINT flags = 0;
		#ifdef CFG_DEBUGENG
		flags = D3DCOMPILE_DEBUG;
		#endif

		ID3DBlob* compilerErrors;
		std::vector<D3D_SHADER_MACRO> macros;
		std::vector<std::string> macroNames;

		int i = 0;
		std::string currentMacro;
		while (defines[i] != '\0')
		{
			while (defines[i] != ' ')
			{
				if (defines[i] == '\0') { i--; break; }

				currentMacro += defines[i];
				i++;
			}
			macroNames.emplace_back(currentMacro);
			currentMacro.clear();
			i++;
		}

		for (auto& string : macroNames)
		{
			macros.push_back({ string.c_str(), "1" });
		}
		macros.push_back({ NULL });

		HRESULT hr = D3DCompileFromFile(wfile.c_str(), macros.data(), NULL, "main", "gs_5_0", flags, NULL, &m_Blob, &compilerErrors);
		if (FAILED(hr))
		{
			auto errorString = std::string((char*)compilerErrors->GetBufferPointer());
			VX_ERROR(LogGraphicsAPI, "Geometry Shader Compilation failed: {0}", errorString);
			compilerErrors->Release();
			__debugbreak();

			GraphicsContext::Get()->UnregisterPrimitive(this);
			throw std::exception("Failed to compile shader!");
		}

		context->GetDevice()->CreateGeometryShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), NULL, &m_Shader);
	}
}
