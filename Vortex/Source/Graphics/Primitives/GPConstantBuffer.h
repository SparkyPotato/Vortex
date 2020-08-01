#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	enum class ConstantBufferTarget
	{
		VertexShader,
		PixelShader
	};

	class GPConstantBuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPConstantBuffer() {}
		
		static GPConstantBuffer* Create(void* data, int size, ConstantBufferTarget target);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
		
		virtual void Set(void* newData) = 0;
	};
}
