#pragma once

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#ifdef _WIN32
#define MS_PLATFORM_WIN32
#elif TARGET_IPHONE_SIMULATOR
#define MS_PLATFORM_IOS_SIMULATOR
#elif TARGET_OS_IPHONE
#define MS_PLATFORM_IOS
#elif TARGET_OS_MAC
#define MS_PLATFORM_OSX
#endif
