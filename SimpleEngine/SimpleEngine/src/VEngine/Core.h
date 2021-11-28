#pragma once

#define BIT(x) (1 << x)

#define VENGINE_ASSERT(x,...) {if(!x){VENGINE_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
#define VENGINE_CORE_ASSERT(x,...) {if(!x){VENGINE_CORE_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
