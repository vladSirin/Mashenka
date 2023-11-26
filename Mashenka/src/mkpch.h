#pragma once

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
