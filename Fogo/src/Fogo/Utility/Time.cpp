#include  "Time.h"

using namespace std;
using namespace std::chrono;
using namespace Fogo::Utility;

float Time::getSeconds() const {
	return static_cast<float>(duration_cast<nanoseconds>(__ended_time - __begun_time).count()) / nano::den;
}

void Time::start() {
	__begun_time = GetCurrent();
}

void Time::stop() {
	__ended_time = GetCurrent();
	__elapsed_time = getSeconds();
}

Time & Time::getInstance() {
	static Time time;
	return time;
}

Time::Time() : __begun_time({}), __ended_time({}) {

}

float Time::GetSeconds() {
	return getInstance().getSeconds();
}

void Time::Start() {
	getInstance().start();
}

void Time::Stop() {
	getInstance().stop();
}

time_point<system_clock> Time::GetCurrent()
{
	return system_clock::now();
}

float Fogo::Utility::Time::GetElapsedTime()
{
	return getInstance().__elapsed_time;
}
