#pragma once

DECLARE_LOGGER(LogGraphicsAPI);

namespace Vortex
{
	/*
		Base class of Graphics Primitives.
	*/
	class GraphicsPrimitive
	{
	public:
		virtual ~GraphicsPrimitive() {}

		/*
			Bind the primitive to the pipeline.
		*/
		virtual void Bind() = 0;
		/*
			Recreate the primitive.
		*/
		virtual void Recreate() = 0;
	};
}
