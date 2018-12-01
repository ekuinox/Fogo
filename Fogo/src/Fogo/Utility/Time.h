#pragma once

#include <chrono>

namespace Fogo::Utility {
	
	class Time {
	private:
		using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
		
		TimePoint __begun_time;
		TimePoint __ended_time;

		Time();
		
		float getSeconds() const;
		void start();
		void stop();
		
		static Time & getInstance();

	public:
		Time(const Time &) = delete;
		Time& operator=(const Time &) = delete;

		static float GetSeconds();
		static void Start();
		static void Stop();
		static TimePoint GetCurrent();
	};

} // namespace Fogo