#pragma once
#include <Core/Module.h>
#include <Math/Matrix.h>
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPConstantBuffer.h>
#include <Graphics/Primitives/GPVertexShader.h>
#include <Graphics/Primitives/GPGeometryShader.h>
#include <Graphics/Primitives/GPPixelShader.h>
#include <im3d.h>

DECLARE_LOGGER(Log3DUI);

namespace Vortex
{
	/*
		Module that deals with Im3d rendering.
	*/
	class VX3dUi : public Module
	{
	public:
		VX3dUi();
		virtual ~VX3dUi();

		virtual void Startup() override;
		virtual void Shutdown() override;
		
		virtual void Tick(float deltaTime) override;
		void Draw();

		virtual void Quit() override;

		virtual void OnConsoleCommand(ConsoleCommand command) override;

		void SetViewportTopLeft(const Math::Vector& topLeft) { m_ViewportTopLeft = topLeft; }

	private:
		Math::Vector m_ViewportTopLeft;
		Math::Vector m_ViewportSize;

		struct ConstantBufferData
		{
			Math::Matrix viewProjectionMatrix;
		};

		struct GeometryConstantBuffer
		{
			Math::Vector viewportDimensions;
		};

		unsigned int m_BufferSize = 0;

		VertexLayout m_VertexLayout =
		{
			{ "POSITIONSIZE", ShaderDataType::float4 },
			{ "COLOR", ShaderDataType::col4 }
		};
		GPVertexBuffer* m_Vertices = nullptr;

		ConstantBufferData m_ConstantBufferData;
		GPConstantBuffer* m_ConstantBuffer = nullptr;
		
		GeometryConstantBuffer m_GeometryConstantBufferData;
		GPConstantBuffer* m_GeometryConstantBuffer = nullptr;

		GPVertexShader* m_VertexShaderTriangles = nullptr;
		GPVertexShader* m_VertexShaderNoTriangles = nullptr;

		GPGeometryShader* m_GeometryShaderPoints = nullptr;
		GPGeometryShader* m_GeometryShaderLines = nullptr;

		GPPixelShader* m_PixelShaderPoints = nullptr;
		GPPixelShader* m_PixelShaderLines = nullptr;
		GPPixelShader* m_PixelShaderTriangles = nullptr;
	};
}
