#pragma once

#define BIT(x) (1 << x)
#define VENGINE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define VENGINE_ASSERT(x,...) {if(!x){VENGINE_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
#define VENGINE_CORE_ASSERT(x,...) {if(!x){VENGINE_CORE_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
