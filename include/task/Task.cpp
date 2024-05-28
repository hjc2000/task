#include"Task.h"

std::shared_ptr<task::Task> task::Task::Create(std::function<void()> func, uint16_t stack_depth)
{
	std::shared_ptr<task::Task> task{ new task::Task{} };
	task->_func = func;
	auto f = [](void *param)
	{
		task::Task *task = (task::Task *)param;
		task->_func();
	};

	xTaskCreate(
		f,
		"task::Task",
		stack_depth,
		task.get(),
		1,
		&task->_handle
	);

	// 本类对象内的字段借由拷贝构造函数拷贝给接收返回值的对象
	return task;
}
