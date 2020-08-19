#pragma once
#include <Graphics/Primitives/GPTexture.h>

namespace Vortex
{
	class DX11Texture : public GPTexture
	{
	public:
		DX11Texture(int width, int height);
		DX11Texture(int width, int height, TextureUsage usage);
		DX11Texture(std::string file);

		DX11Texture(ID3D11Texture2D* texture);
		virtual ~DX11Texture();

		virtual void Bind() override;
		virtual void Recreate() override;

		virtual int GetWidth() override { return m_Width; }
		virtual int GetHeight() override { return m_Height; }

		virtual void Resize(int width, int height) override;

		virtual void LoadFromFile(std::string file) override;

		ID3D11Texture2D* GetTexture() { return p_Texture; }

		ID3D11ShaderResourceView* GetShaderResource() { if (p_ShaderResource) return p_ShaderResource; else return nullptr; }

	private:
		void Create(int width, int height, TextureUsage usage);

		int m_Width, m_Height;
		TextureUsage m_Usage;
		ID3D11Texture2D* p_Texture = nullptr;
		ID3D11ShaderResourceView* p_ShaderResource = nullptr;
	};
}
