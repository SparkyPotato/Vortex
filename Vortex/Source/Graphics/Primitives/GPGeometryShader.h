#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	/*
		A geometry shader.
	*/
	class GPGeometryShader : public GraphicsPrimitive
	{
	public:
		virtual ~GPGeometryShader() {}

		/*
			Creates a Geometry shader from the HLSL file given.
		*/
		static GPGeometryShader* Create(std::string file, std::string defines);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Recreate() = 0;
	};
}
