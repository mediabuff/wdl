// debug.hpp
//
// Windows debugging utilities.

#pragma once

#ifdef _DEBUG
#include <windows.h>
#include <crtdbg.h>
#include <atlbase.h>
#endif

#ifdef _DEBUG
#define ASSERT _ASSERTE
#define VERIFY ASSERT
#define VERIFY_(result, expression) ASSERT(result == expression)
#define TRACE ATLTRACE
#else
#define ASSERT __noop
#define VERIFY(expression) (expression)
#define VERIFY_(result, expression) (expression)
#define TRACE __noop
#endif