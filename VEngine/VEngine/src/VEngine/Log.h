#pragma once

#include <memory> // for use shared_ptr<>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace VEngine 
{
	class VENGINE_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;//for engine use
		static std::shared_ptr<spdlog::logger> s_ClientLogger;//for client app use
	};
}

// Core log macros
#define VENGINE_CORE_TRACE(...)    ::VEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VENGINE_CORE_INFO(...)     ::VEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VENGINE_CORE_WARN(...)     ::VEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VENGINE_CORE_ERROR(...)    ::VEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VENGINE_CORE_CRITICAL(...) ::VEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define VENGINE_TRACE(...)         ::VEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VENGINE_INFO(...)          ::VEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define VENGINE_WARN(...)          ::VEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VENGINE_ERROR(...)         ::VEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define VENGINE_CRITICAL(...)      ::VEngine::Log::GetClientLogger()->critical(__VA_ARGS__)