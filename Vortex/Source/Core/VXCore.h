#pragma once

namespace Vortex
{
	class VXCore
	{
	public:
		static void Startup();
		static int Shutdown();

		static VXCore* GetCore() { return s_Core; }

		void RunCoreLoop();
		void Quit();

	private:
		VXCore();
		~VXCore();

		static VXCore* s_Core;

		std::mutex m_TickMutex;
		bool m_IsTicking;
	};
}
