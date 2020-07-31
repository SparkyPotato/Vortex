#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define PLATFORM_WINDOWS
	#else
		#error "Vortex is 64-bit only!"
	#endif
#elif
    #ifdef __APPLE__
        #include <TargetConditionals.h>
        #if TARGET_PLATFORM_OSX == 1
            #define PLATFORM_MAC
        #else
            #error "macOS is the only Apple OS supported!"
        #endif
    #endif
#else
	#error "Windows is the only supported platform as of yet!"
#endif
