#include"Task.h"
#include<stdexcept>

std::shared_ptr<task::Task> task::Task::Create(std::function<void()> func, uint16_t stack_depth)
{
	if (func == nullptr)
	{
		throw std::invalid_argument { "func 不能为 nullptr" };
	}

	std::shared_ptr<task::Task> task { new task::Task { } };
	task->_func = func;
	auto f = [](void *param)
	{
		try
		{
			task::Task *task = (task::Task *)param;
			task->_func();
		}
		catch (...)
		{

		}

		// 任务要退出必须删除自身，否则会内存泄漏
		vTaskDelete(nullptr);
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
