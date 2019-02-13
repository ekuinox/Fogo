#pragma once

#include <map>
#include <thread>

namespace Fogo::Utility {

	class TaskScheduler final {
	public:
		using Function = std::function<void(void)>;

		enum class Priority : unsigned char {
			Highest,
			Higher,
			Medium,
			Lower,
			Lowest
		};

		enum class ThreadState {
			Dead,
			Running,
			Finished
		};

	private:
		std::map<Priority, std::vector<Function>> tasks;
		std::thread taskThread;
		ThreadState state;

		static TaskScheduler * instance;

		TaskScheduler();
		virtual ~TaskScheduler();
		void addTask(const Priority & priority, const Function & func);
		void addTask(const Function & func);
		void execTasks();
		ThreadState getState() const;
		bool isThreadRunning() const;

	public:
		static TaskScheduler & GetInstance();
		static TaskScheduler & Create();
		static void Destroy();
		static void AddTask(const Priority & priority, const Function & func);
		static void AddTask(const Function & func);
		static void ExecTasks();
		static ThreadState GetState();
		static bool IsThreadRunning();

	};
}
