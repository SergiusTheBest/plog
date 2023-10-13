#pragma once

#if defined(__cpp_constexpr) && (!defined(__MINGW32__) || defined(__MINGW64_VERSION_MAJOR))
#   include "doctest/2.4.11/doctest.h"  // C++11 and higher
#else
#   include "doctest/1.2.9/doctest.h"   // pre C++11
#endif

#include <plog/Log.h>
#include "TestAppender.h"

#ifdef __has_include
#   if __has_include(<version>)
#       include <version>
#   endif
#endif
