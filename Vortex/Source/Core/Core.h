#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define PLATFORM_WINDOWS
	#else
		#error "Vortex is 64-bit only!"
	#endif
#else
	#error "Windows is the only supported platform as of yet!"
#endif
