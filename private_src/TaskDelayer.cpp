#include "TaskDelayer.h"
#include <base/unit/Hz.h>
#include <base/unit/Seconds.h>
#include <bsp-interface/di/delayer.h>
#include <bsp-interface/di/systick.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <FreeRTOS.h>
#include <task.h>

task::TaskDelayer &task::TaskDelayer::Instance()
{
	class Getter :
		public bsp::TaskSingletonGetter<TaskDelayer>
	{
	public:
		std::unique_ptr<TaskDelayer> Create() override
		{
			return std::unique_ptr<TaskDelayer>{new TaskDelayer{}};
		}
	};

	Getter g;
	return g.Instance();
}

void task::TaskDelayer::Delay(std::chrono::microseconds microseconds)
{
	// 大于 1000 的微秒转换为毫秒，利用 Delay(std::chrono::milliseconds milliseconds) 重载。
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(microseconds);
	if (ms.count() > 0)
	{
		Delay(ms);
	}

	// 剩余的小于 1000 部分的微秒
	microseconds -= ms;
	DI_SysTick().Delay(microseconds);
}

void task::TaskDelayer::Delay(std::chrono::milliseconds milliseconds)
{
	// 如果调度器不在运行，则使用 SysTickClock::Delay
	if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
	{
		DI_SysTick().Delay(milliseconds);
		return;
	}

	base::Seconds freertos_tick_interval{base::Hz{configTICK_RATE_HZ}};
	base::Seconds period{milliseconds};

	// 先让 freertos 把整数个滴答延时完毕
	int64_t tick_count = static_cast<int64_t>(period / freertos_tick_interval);
	if (tick_count > 0)
	{
		vTaskDelay(tick_count);
	}

	// 剩下的需要延时的时间小于 freertos 的滴答周期，无法用 freertos 延时
	base::Seconds remain = period - freertos_tick_interval * tick_count;
	if (remain > 0)
	{
		DI_SysTick().Delay(static_cast<std::chrono::milliseconds>(remain));
	}
}

void task::TaskDelayer::Delay(std::chrono::seconds seconds)
{
	Delay(std::chrono::milliseconds{seconds});
}
