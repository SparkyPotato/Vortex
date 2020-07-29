#pragma once
#include <Graphics/Primitives/GPPixelShader.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>

namespace Vortex
{
	class DX11PixelShader : public GPPixelShader
	{
	public:
		DX11PixelShader(std::string file);
		virtual ~DX11PixelShader();

		virtual void Bind() override;
		virtual void Recreate() override;

	private:
		void Create(std::string file);

		ID3D11PixelShader* m_Shader;
		ID3DBlob* m_Blob;
		std::string m_File;
	};
}
