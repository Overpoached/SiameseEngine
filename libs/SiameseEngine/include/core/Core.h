#pragma once

#if defined(SE_DEBUG) || defined(SE_RELWITHDEBINFO)
#define SE_ASSERT(expr, msg)																					\
	do {																										\
		if(!(expr)){																							\
			LoggerService::Get()->Error("Assertion failed: %s\nFile: %s\nLine: %s", msg, __FILE__, __LINE__);	\
			__debugbreak();																						\
		}																										\
	} while (0)
#else
#define SE_ASSERT(expr, msg) ((void)0)
#endif

//#define CLOCK_MILLISECOND
#define CLOCK_MICROSECOND
//#define CLOCK_NANOSECOND