#pragma once

namespace VEngine
{
	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f) : m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSecondes() const { return m_Time; }
		float GetMillSeconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}
