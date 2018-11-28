#pragma once

#include <chrono>

namespace Fogo {
	namespace chrono = std::chrono;

	class Time
	{
	public:
		class Timer
		{
		private:
			using clock = chrono::system_clock;
			using time_point = chrono::time_point<clock>;

			time_point __started_time;
			time_point __ended_time;

		public:
			const time_point now() const
			{
				return clock::now();
			}

			void start()
			{
				__started_time = now();
			}

			void end()
			{
				__ended_time = now();
			}

			float getSeconds() const
			{
				return static_cast<float>(chrono::duration_cast<chrono::nanoseconds>(__ended_time - __started_time).count()) / std::nano::den;
			}
		};

	private:
		static Timer __timer;

	public:
		static Timer CreateTimer()
		{
			return Timer();
		}

		static void Start()
		{
			__timer.start();
		}

		static void End()
		{
			__timer.end();
		}

		static float GetSeconds()
		{
			return __timer.getSeconds();
		}
	};

	Time::Timer Time::__timer = Time::Timer();

} // namespace Fogo