#pragma once

// Suppress warnings
#pragma warning(disable : 4251) //supress warnings about dll interface
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Mashenka/Core/Log.h"
#ifdef MK_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM "Mashenka/Vendor/Glad/include/glad/glad.h"

