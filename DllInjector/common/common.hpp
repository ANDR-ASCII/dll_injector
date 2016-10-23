#pragma once
#include <cassert>

#ifndef NDEBUG
#define VERIFY(F) assert(F)
#else 
#define VERIFY(F) F
#endif

namespace Common
{

}