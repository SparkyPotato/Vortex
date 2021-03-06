#pragma once
#include <Graphics/Primitives/GPVertexShader.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	class DX11VertexShader : public GPVertexShader
	{
	public:
		DX11VertexShader(std::string file, std::string defines);
		virtual ~DX11VertexShader();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Recreate() override;

		ID3DBlob* GetBlob() { return m_Blob; }

	private:
		void Create(std::string file, std::string defines);

		ID3D11VertexShader* m_Shader;
		ID3DBlob* m_Blob;
		std::string m_File;
		std::string m_Defines;
	};
}
