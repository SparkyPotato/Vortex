#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <string>

namespace Vortex
{
	class GPVertexShader : public GraphicsPrimitive
	{
	public:
		virtual ~GPVertexShader() {}

		static GPVertexShader* Create(std::string file);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
	};
}
