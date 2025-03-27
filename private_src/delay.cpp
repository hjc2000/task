#include "base/task/delay.h"
#include "base/unit/Hz.h"
#include "base/unit/Seconds.h"
#include "bsp-interface/di/systick.h"
#include "FreeRTOS.h"
#include "task.h"
#include <chrono>

void base::task::Delay(std::chrono::seconds const &time)
{
	Delay(std::chrono::milliseconds{time});
}

void base::task::Delay(std::chrono::milliseconds const &time)
{
	// 如果调度器不在运行，则使用 SysTickClock::Delay
	if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
	{
		DI_SysTick().Delay(time);
		return;
	}

	base::Seconds freertos_tick_interval{base::Hz{configTICK_RATE_HZ}};
	base::Seconds period{time};

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

void base::task::Delay(std::chrono::microseconds const &time)
{
	// 大于 1000 的微秒转换为毫秒，利用 Delay(std::chrono::milliseconds milliseconds) 重载。
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
	if (ms.count() > 0)
	{
		Delay(ms);
	}

	// 剩余的小于 1000 部分的微秒
	std::chrono::microseconds remain = time - ms;
	DI_SysTick().Delay(remain);
}

void base::task::Delay(std::chrono::nanoseconds const &time)
{
	Delay(std::chrono::duration_cast<std::chrono::microseconds>(time));
}
