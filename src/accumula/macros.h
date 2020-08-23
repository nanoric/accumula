#pragma once

#include <boost/assert.hpp>

#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG)
#    if defined(NDEBUG)
#        pragma warning("Unable to determine DEBUG/RELEASE")
#    endif
#    define ACCUMULA_IS_DEBUG() 1
#    define ACCUMULA_IS_RELEASE() 0
#else
#    define ACCUMULA_IS_DEBUG() 0
#    define ACCUMULA_IS_RELEASE() 1
#endif
