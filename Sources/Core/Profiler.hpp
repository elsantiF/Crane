#pragma once
#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>

#ifdef _MSC_VER
#define FUNCTION_SIGNATURE __FUNCTION__
#else
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif

#define PROFILE_DEPTH 15
#define PROFILE_SCOPE() ZoneScopedNS(FUNCTION_SIGNATURE, PROFILE_DEPTH)

#define PROFILE_MEM_ALLOC(ptr, size) TracyAllocS(ptr, size, PROFILE_DEPTH)
#define PROFILE_MEM_FREE(ptr) TracyFreeS(ptr, PROFILE_DEPTH)

#else

#define PROFILE_DEPTH
#define PROFILE_SCOPE()

#define PROFILE_MEM_ALLOC(ptr, size)
#define PROFILE_MEM_FREE(ptr)

#endif