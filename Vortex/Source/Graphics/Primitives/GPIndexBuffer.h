#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	/*
		An index buffer.
	*/
	class GPIndexBuffer : public GraphicsPrimitive
	{
	public:
		virtual ~GPIndexBuffer() {}

		/*
			Creates an index buffer from `count` indices.
		*/
		static GPIndexBuffer* Create(unsigned int* indices, int count);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Recreate() = 0;

		virtual int GetSize() = 0;
	};
}
