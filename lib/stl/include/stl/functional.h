#ifndef __STL_FUNCTIONAL_H_INCLUDED__
#define __STL_FUNCTIONAL_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <functional>
#elif defined(USE_EASTL_STL)
	#include <EASTL/functional.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
