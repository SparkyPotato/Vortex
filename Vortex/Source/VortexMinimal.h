#pragma once

/*
	Contains minimal functionality to create an application with Vortex.
*/

// Core platform definitions - including so that the application can write platform-specific code.
#include <Core/Core.h>

// Input Codes for application input.
#include <Core/InputCode.h>

// Logging functionality.
#include <Core/Logger.h>

// Profiling functionality
#include <Core/Profiler.h>

// Vortex Core Module
#include <Core/Modules/VXCore.h>

// Application from which to derive.
#include <Core/Application.h>

// Application layers
#include <Core/Layers/Layer.h>
