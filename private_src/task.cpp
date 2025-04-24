#include "base/task/task.h"
#include "base/Console.h"
#include "base/peripheral/systick/systick.h"
#include "base/string/define.h"
#include "base/task/TaskCompletionSignal.h"
#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>

namespace
{
	///
	/// @brief 默认的任务栈大小。
	///
	///
	size_t _default_stack_size = 1024 * 8;

	///
	/// @brief 默认的任务优先级。
	///
	///
	uint32_t _default_priority = 1;

	struct TaskContext
	{
		TaskHandle_t handle{};
		std::function<void()> func{};
		std::shared_ptr<base::TaskCompletionSignal> task_completion_signal{new base::TaskCompletionSignal{false}};
	};

} // namespace

/* #region 任务栈大小 */

///
/// @brief 默认的任务栈大小。
///
/// @return int64_t
///
size_t base::task::default_task_stack_size()
{
	return _default_stack_size;
}

///
/// @brief 设置默认的任务栈大小。
///
/// @param value
///
void base::task::set_default_task_stack_size(size_t value)
{
	_default_stack_size = value;
}

/* #endregion */

/* #region 任务优先级 */

///
/// @brief 任务的默认优先级。
///
/// @return
///
uint32_t base::task::default_priority()
{
	return _default_priority;
}

///
/// @brief 设置任务的默认优先级。
///
/// @param value
///
void base::task::set_default_priority(uint32_t value)
{
	_default_priority = value;
}

/* #endregion */

/* #region run */

///
/// @brief 运行一个任务。
///
/// @note 实现者需要将后台线程实现为捕获所有异常，输出错误消息。
///
/// @param func 任务函数。
///
/// @return
///
std::shared_ptr<base::TaskCompletionSignal> base::task::run(std::function<void()> const &func)
{
	return base::task::run("",
						   _default_priority,
						   _default_stack_size,
						   func);
}

///
/// @brief 运行一个任务。
///
/// @note 实现者需要将后台线程实现为捕获所有异常，输出错误消息。
///
/// @param stack_size 任务栈大小。单位：字节。有些平台例如 freertos 需要这个参数。
/// @param func 任务函数。
///
/// @return
///
std::shared_ptr<base::TaskCompletionSignal> base::task::run(size_t stack_size,
															std::function<void()> const &func)
{
	return base::task::run("",
						   _default_priority,
						   stack_size,
						   func);
}

///
/// @brief 运行一个任务。
///
/// @param priority 任务优先级。
/// @param stack_size 任务栈大小。单位：直接。
/// @param func 任务函数。
/// @return
///
std::shared_ptr<base::TaskCompletionSignal> base::task::run(uint32_t priority,
															size_t stack_size,
															std::function<void()> const &func)
{
	return base::task::run("",
						   priority,
						   stack_size,
						   func);
}

std::shared_ptr<base::TaskCompletionSignal> base::task::run(std::string const &task_name,
															uint32_t priority,
															size_t stack_size,
															std::function<void()> const &func)
{
	if (func == nullptr)
	{
		throw std::invalid_argument{CODE_POS_STR + "func 不能为 nullptr"};
	}

	TaskContext *context = new TaskContext{};
	context->func = func;

	auto task_func = [](void *param)
	{
		TaskContext *context = reinterpret_cast<TaskContext *>(param);

		try
		{
			context->func();
		}
		catch (std::exception const &e)
		{
			base::console.WriteErrorLine("任务函数发生了异常：");
			base::console.WriteErrorLine(e.what());
		}
		catch (...)
		{
			base::console.WriteErrorLine("任务函数发生了未知的异常。");
		}

		context->task_completion_signal->SetResult();
		delete context;

		// 任务要退出必须删除自身，否则会内存泄漏
		vTaskDelete(nullptr);
	};

	xTaskCreate(task_func,
				task_name.c_str(),
				std::ceil<uint32_t>(stack_size / sizeof(size_t)),
				context,
				priority,
				&context->handle);

	return context->task_completion_signal;
}

/* #endregion */

/* #region 调度管理 */

///
/// @brief 启动调度。
///
/// @note 有的平台需要手动启动调度。例如 freertos. 如果平台不需要手动启动调度，
/// 则不实现此函数。
///
void base::task::start_scheduler()
{
	// 启动调度前需要设置回调函数，这个回调函数由 SysTick 定时时间到中断触发，
	// 在这里面定期调用 xPortSysTickHandler，它是 freertos 时间的来源，没有它，
	// freertos 的与时间相关的功能都不能工作。
	base::systick::set_elapsed_handler(
		[]()
		{
			if (base::task::scheduler_has_started())
			{
				xPortSysTickHandler();
			}
		});

	vTaskStartScheduler();
}

///
/// @brief 任务调度已经开始了。
///
/// @return
///
bool base::task::scheduler_has_started()
{
	return xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED;
}

///
/// @brief 暂停所有任务的调度。
///
///
void base::task::suspend_all_task_scheduler()
{
	vTaskSuspendAll();
}

///
/// @brief 恢复所有任务的调度。
///
///
void base::task::resume_all_task_scheduler()
{
	xTaskResumeAll();
}

/* #endregion */

extern "C"
{
	void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
	{
		base::console.WriteErrorLine(std::string{"任务栈发生溢出。，任务名："} + pcTaskName);
	}
}
