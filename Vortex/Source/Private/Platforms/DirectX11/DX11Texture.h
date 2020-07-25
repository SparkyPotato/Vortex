#pragma once
#include <Graphics/Primitives/GPTexture.h>

namespace Vortex
{
	class DX11Texture : public GPTexture
	{
	public:
		DX11Texture(int width, int height);
		DX11Texture(int width, int height, TextureUsage usage);
		DX11Texture(ID3D11Texture2D* texture);
		virtual ~DX11Texture();

		virtual void Bind() override;
		virtual void Recreate() override;

		ID3D11Texture2D* GetTexture() { return p_Texture; }

	private:
		void Create(int width, int height, TextureUsage usage);

		int m_Width, m_Height;
		TextureUsage m_Usage;
		ID3D11Texture2D* p_Texture;
	};
}
