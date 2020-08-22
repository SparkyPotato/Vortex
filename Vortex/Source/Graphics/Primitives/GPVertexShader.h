#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	/*
		A vertex shader. 
	*/
	class GPVertexShader : public GraphicsPrimitive
	{
	public:
		virtual ~GPVertexShader() {}

		/*
			Creates a vertex shader from the HLSL file given.
		*/
		static GPVertexShader* Create(std::string file, std::string defines);

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
	};
}
