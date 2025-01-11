#include "TaskManager.h"

bsp::TaskManager &bsp::TaskManager::Instance()
{
	class Getter :
		public base::SingletonGetter<TaskManager>
	{
	public:
		std::unique_ptr<TaskManager> Create() override
		{
			return std::unique_ptr<TaskManager>{new TaskManager{}};
		}

		void Lock() override
		{
			vTaskSuspendAll();
		}

		void Unlock() override
		{
			xTaskResumeAll();
		}
	};

	Getter g;
	return g.Instance();
}

std::shared_ptr<bsp::ITask> bsp::TaskManager::Create(std::function<void()> func, uint32_t stack_size)
{
	return task::FreeTask::Create(func, stack_size);
}

void bsp::TaskManager::StartScheduler()
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

void bsp::TaskManager::SuspendAllTask()
{
	vTaskSuspendAll();
}

void bsp::TaskManager::ResumeAllTask()
{
	xTaskResumeAll();
}
