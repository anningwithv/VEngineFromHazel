#pragma once

namespace VEngine
{
	class GameMode
	{
	public:
		enum class Mode
		{
			D2 = 0,
			D3 = 1
		};

		static void SetGameMode(Mode mode) { s_Mode = mode; }
		static Mode GetGameMode() { return s_Mode; }

		static Mode s_Mode;
	};
}
