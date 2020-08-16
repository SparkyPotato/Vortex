#pragma once

// Include first so that platform macros are defined everywhere
#include <Core/Core.h>

// So that logging, input, and profiling can be accessed from anywhere in the engine.
#include <Core/InputCode.h>
#include <Core/Logger.h>
#include <Core/Profiler.h>

// STL includes
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <functional>
