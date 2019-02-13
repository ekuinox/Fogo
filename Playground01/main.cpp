#include <iostream>
#include "./TaskScheduler.h"

/*

C++�̋@�\�Ƃ������Ă݂�����������邩�Ȃǂ��ꂪ�����ꏊ�@���̂P

*/

auto main() -> int {
	
	TaskScheduler::Create();

	TaskScheduler::AddTask(TaskScheduler::Priority::Higher, [] {
		std::cout << "[Higher] Hello" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "[Higher] Goodbye" << std::endl;
	});

	TaskScheduler::AddTask(TaskScheduler::Priority::Highest, [] {
		std::cout << "[Highest] Hello #1" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "[Highest] Goodbye #1" << std::endl;
	});

	TaskScheduler::AddTask(TaskScheduler::Priority::Highest, [] {
		std::cout << "[Highest] Hello #2" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "[Highest] Goodbye #2" << std::endl;
	});

	TaskScheduler::ExecTasks();

	std::cout << "�ɂイ��傭�܂�" << std::endl;
	getchar();

	if (!TaskScheduler::IsThreadRunning()) {
		std::cout << "�ɂ��" << std::endl;
		TaskScheduler::ExecTasks();
	}

	std::cout << "�ɂイ��傭�܂�" << std::endl;
	getchar();

	return 0;
}
