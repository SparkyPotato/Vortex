#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	/*
		A pixel shader.
	*/
	class GPPixelShader : public GraphicsPrimitive
	{
	public:
		virtual ~GPPixelShader() {}

		/*
			Create the pixel shader from the HLSL file.
		*/
		static GPPixelShader* Create(std::string file);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
	};
}
