#pragma once
#include <Graphics/Primitives/GPGeometryShader.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	class DX11GeometryShader : public GPGeometryShader
	{
	public:
		DX11GeometryShader(std::string file, std::string defines);
		virtual ~DX11GeometryShader();

		virtual void Bind() override;
		virtual void Recreate() override;

		ID3DBlob* GetBlob() { return m_Blob; }

	private:
		void Create(std::string file, std::string defines);

		ID3D11GeometryShader* m_Shader;
		ID3DBlob* m_Blob;
		std::string m_File;
		std::string m_Defines;
	};
}
