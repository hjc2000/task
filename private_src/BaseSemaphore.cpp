#include "BaseSemaphore.h"
#include <base/string/define.h>
#include <base/unit/Hz.h>

task::BaseSemaphore::BaseSemaphore(int32_t initial_count)
{
	_semaphore = xSemaphoreCreateCounting(60000, initial_count);
}

void task::BaseSemaphore::Release(int32_t count)
{
	if (count <= 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "count 不能 <= 0."};
	}

	for (int64_t i = 0; i < count; i++)
	{
		xSemaphoreGive(_semaphore);
	}
}

void task::BaseSemaphore::ReleaseFromISR(int32_t count)
{
	if (count <= 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "count 不能 <= 0."};
	}

	for (int64_t i = 0; i < count; i++)
	{
		xSemaphoreGiveFromISR(_semaphore, &_higher_priority_task_woken);
	}
}

void task::BaseSemaphore::Acquire()
{
	xSemaphoreTake(_semaphore, portMAX_DELAY);
}

bool task::BaseSemaphore::TryAcquire(base::Seconds const &timeout)
{
	if (timeout < 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "超时时间不能 <= 0."};
	}

	base::Seconds freertos_tick_interval{base::Hz{configTICK_RATE_HZ}};
	int64_t tick_count = static_cast<int64_t>(timeout / freertos_tick_interval);
	if (tick_count >= static_cast<int64_t>(portMAX_DELAY))
	{
		throw std::invalid_argument{CODE_POS_STR + "等待时间过长。"};
	}

	bool result = xSemaphoreTake(_semaphore, tick_count);
	return result;
}

std::shared_ptr<base::IBaseSemaphore> base::CreateBaseSemaphore(int32_t initial_count)
{
	return std::shared_ptr<base::IBaseSemaphore>{new task::BaseSemaphore{initial_count}};
}
