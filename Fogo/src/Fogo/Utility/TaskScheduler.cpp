#include "./TaskScheduler.h"

using Fogo::TaskScheduler;

TaskScheduler::TaskScheduler() : state(ThreadState::Dead) {

}

TaskScheduler::~TaskScheduler() {
	if (state == ThreadState::Running && taskThread.joinable()) {
		taskThread.join();
	}
}

void TaskScheduler::addTask(const Priority & priority, const Function & func) {
	tasks[priority].emplace_back(func);
}

void TaskScheduler::addTask(const Function & func) {
	addTask(Priority::Medium, func);
}

void TaskScheduler::execTasks() {
	if (state == ThreadState::Running) return;
	if (state == ThreadState::Finished && taskThread.joinable()) {
		taskThread.join();
	}

	for (const auto & [priority, tasks] : tasks) {
		for (const auto & task : tasks) {
			runningTasks[priority].emplace_back(task);
		}
	}

	tasks.clear();

	taskThread = std::thread([&] {
		state = ThreadState::Running;
		for (const auto &[priority, tasks] : runningTasks) {
			for (const auto & task : tasks) {
				task();
			}
		}
		runningTasks.clear();
		state = ThreadState::Finished;
	});
}

TaskScheduler::ThreadState TaskScheduler::getState() const {
	return state;
}

bool TaskScheduler::isThreadRunning() const {
	return state == ThreadState::Running;
}

TaskScheduler & TaskScheduler::GetInstance() {
	return * instance;
}

TaskScheduler & TaskScheduler::Create() {
	instance = new TaskScheduler;
	return * instance;
}

void TaskScheduler::Destroy() {
	delete instance;
	instance = nullptr;
}

void TaskScheduler::AddTask(const Priority & priority, const Function & func) {
	GetInstance().addTask(priority, func);
}

void TaskScheduler::AddTask(const Function & func) {
	GetInstance().addTask(func);
}

void TaskScheduler::ExecTasks() {
	GetInstance().execTasks();
}

TaskScheduler::ThreadState TaskScheduler::GetState()
{
	return GetInstance().getState();
}

bool TaskScheduler::IsThreadRunning() {
	return GetInstance().isThreadRunning();
}

TaskScheduler * TaskScheduler::instance = nullptr;
