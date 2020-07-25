#pragma once

namespace Vortex
{
	class GraphicsPrimitive
	{
	public:
		virtual ~GraphicsPrimitive() {}

		virtual void Bind() = 0;
		virtual void Recreate() = 0;
	};
}
