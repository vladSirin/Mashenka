
#pragma once

#include "Mashenka/Core/Base.h"
#include "Mashenka/Core/Log.h"
#include <filesystem>

#ifdef MK_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define MK_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { MK##type##ERROR(msg, __VA_ARGS__); MK_DEBUGBREAK(); } }
#define MK_INTERNAL_ASSERT_WITH_MSG(type, check, ...) MK_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define MK_INTERNAL_ASSERT_NO_MSG(type, check) MK_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", MK_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define MK_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define MK_INTERNAL_ASSERT_GET_MACRO(...) MK_EXPAND_MACRO( MK_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, MK_INTERNAL_ASSERT_WITH_MSG, MK_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define MK_ASSERT(...) MK_EXPAND_MACRO( MK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define MK_CORE_ASSERT(...) MK_EXPAND_MACRO( MK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define MK_ASSERT(...)
#define MK_CORE_ASSERT(...)
#endif
