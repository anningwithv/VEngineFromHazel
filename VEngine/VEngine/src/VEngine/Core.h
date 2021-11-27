#pragma once
#ifdef VENGINE_PLATFORM_WINDOWS
#ifdef VENGINE_BUILD_DLL
#define VENGINE_API __declspec(dllexport)
#else
#define VENGINE_API __declspec(dllimport)
#endif // HZ_BUILD_DLL
#else
#error VEngine only support Windows!
#endif // !HZ_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
