#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>
#include <string>

namespace Vortex
{
	class GPPixelShader : public GraphicsPrimitive
	{
	public:
		virtual ~GPPixelShader() {}

		static GPPixelShader* Create(std::string file);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
	};
}
