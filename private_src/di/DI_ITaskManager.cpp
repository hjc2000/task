#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/systick.h>
#include <bsp-interface/di/task.h>
#include <FreeTask.h>

/// @brief 创建一个任务
/// @param stack_size 任务栈大小。单位：字。
/// @note 对于 32 位宽的 CPU ，一个字是 32 位，即 4 个字节。
/// @param func 任务函数
std::shared_ptr<bsp::ITask> DI_CreateTask(uint32_t stack_size, std::function<void()> func)
{
	return task::FreeTask::Create(func, stack_size);
}

/// @brief 启动调度。本函数会持续阻塞。
void DI_StartScheduler()
{
	/* 启动调度前需要设置回调函数，这个回调函数由 SysTick 定时时间到中断触发，
	 * 在这里面定期调用 xPortSysTickHandler，它是 freertos 时间的来源，没有它，
	 * freertos 的与时间相关的功能都不能工作。
	 */
	DI_SysTick().SetElapsedHandler(
		[]()
		{
			if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
			{
				xPortSysTickHandler();
			}
		});

	vTaskStartScheduler();
}

/// @brief 暂停所有调度。
/// @warning 实现本函数时禁止使用动态内存分配，因为堆也需要本函数。
void DI_SuspendAllTask()
{
	vTaskSuspendAll();
}

/// @brief 恢复所有调度。
/// @note 如果调度器没有启动，本函数不会启动调度器。
/// @warning 实现本函数时禁止使用动态内存分配，因为堆也需要本函数。
void DI_ResumeAllTask()
{
	xTaskResumeAll();
}
