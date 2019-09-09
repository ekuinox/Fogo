#pragma once

#include <chrono>
#include <unordered_map>
#include <functional>

namespace Fogo {
	
	class Time {
	private:
		using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
		
		TimePoint __begun_time;
		TimePoint __ended_time;
		float __elapsed_time;

		struct Timer {
			std::function<void(void)> func;
			float span;
			TimePoint whenStarted;
		};
		std::unordered_map<const char *, Timer> __timers;

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
		static float GetElapsedTime();
		static void RegisterTimer(const char * key, float time, const std::function<void(void)> & func);
		static void CheckTimers();



		// éûä‘ç∑ÇãÅÇﬂÇÈ
		class Counter {
		private:
			TimePoint begun;

		public:
			Counter();
			float operator()() const;

		};
	};

} // namespace Fogo