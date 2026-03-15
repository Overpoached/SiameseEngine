#pragma once

#ifdef USER_SYSTEM_LIST_HEADER
#include USER_SYSTEM_LIST_HEADER
#else
#define USER_SYSTEM_LIST(X)
#endif

namespace sengine
{
#define BASE_SYSTEM_LIST(X)\
	X(Undefined)	\
	X(General)		\
	X(Core)			\
	X(Window)		\
	X(Input)		\
	X(Render)		\
	X(Physics)		\
	X(Audio)		\
	X(Game)			\
	X(AI)			\
	X(Network)

#define SYSTEMLIST(X)\
	BASE_SYSTEM_LIST(X)\
	USER_SYSTEM_LIST(X)

	enum class Systems
	{
#define ENUM_ELEMENT(name) name,
		SYSTEMLIST(ENUM_ELEMENT)
#undef ENUM_ELEMENT
		Count
	};

	constexpr const char* SystemNames[] =
	{
#define STRING_ELEMENT(name) #name,
		SYSTEMLIST(STRING_ELEMENT)
#undef STRING_ELEMENT
	};
}

#if defined(SE_DEBUG) || defined(SE_RELWITHDEBINFO)
//if expr is false, debug break
#define SE_ASSERT(expr, ...)																					\
	do {																										\
		if(!(expr)){																							\
			sengine::LoggerService::Get()->Error("Assertion failed at File: {}\nLine: {}", __FILE__, __LINE__);	\
			sengine::LoggerService::Get()->Error(__VA_ARGS__);													\
			__debugbreak();																						\
		}																										\
	} while (0)
#else
#define SE_ASSERT(expr, msg) ((void)0)
#endif

#define UNREFERENCED_PARAMETER(P) (P)
#define NOMINMAX

//#define CLOCK_MILLISECOND
#define CLOCK_MICROSECOND
//#define CLOCK_NANOSECOND

//debug macros
#define SE_MEMORY_DEBUG_LOG 0