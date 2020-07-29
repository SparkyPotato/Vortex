#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	class GPIndexBuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPIndexBuffer() {}

		static GPIndexBuffer* Create(unsigned int* indices, int count);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
	};
}
