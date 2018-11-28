#include  "Time.h"

using namespace std;
using namespace std::chrono;

float Fogo::Time::getSeconds() const {
	return static_cast<float>(duration_cast<nanoseconds>(__ended_time - __begun_time).count()) / nano::den;
}

void Fogo::Time::start() {
	__begun_time = GetCurrent();
}

void Fogo::Time::stop() {
	__ended_time = GetCurrent();
}

Fogo::Time & Fogo::Time::getInstance() {
	static Time time;
	return time;
}

Fogo::Time::Time() : __begun_time({}), __ended_time({}) {

}

float Fogo::Time::GetSeconds() {
	return getInstance().getSeconds();
}

void Fogo::Time::Start() {
	getInstance().start();
}

void Fogo::Time::Stop() {
	getInstance().stop();
}

time_point<system_clock> Fogo::Time::GetCurrent()
{
	return system_clock::now();
}
