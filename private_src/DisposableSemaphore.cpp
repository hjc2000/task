#include "DisposableSemaphore.h"
#include <base/string/define.h>
#include <base/unit/Hz.h>

task::DisposableSemaphore::DisposableSemaphore(int32_t initial_count)
{
	_semaphore = xSemaphoreCreateCounting(60000, initial_count);
}

task::DisposableSemaphore::~DisposableSemaphore()
{
	Dispose();
}

void task::DisposableSemaphore::Dispose()
{
	if (_disposed)
	{
		return;
	}

	_disposed = true;
	Release(10000);
}

void task::DisposableSemaphore::Release(int32_t count)
{
	if (count <= 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "count 不能 <=0."};
	}

	for (int64_t i = 0; i < count; i++)
	{
		xSemaphoreGive(_semaphore);
	}
}

void task::DisposableSemaphore::ReleaseFromISR(int32_t count)
{
	if (count <= 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "count 不能 <=0."};
	}

	for (int64_t i = 0; i < count; i++)
	{
		xSemaphoreGiveFromISR(_semaphore, &_higher_priority_task_woken);
	}
}

void task::DisposableSemaphore::Acquire()
{
	if (_disposed)
	{
		throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
	}

	xSemaphoreTake(_semaphore, portMAX_DELAY);

	if (_disposed)
	{
		throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
	}
}

bool task::DisposableSemaphore::TryAcquire(base::Seconds const &timeout)
{
	if (_disposed)
	{
		throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
	}

	base::Seconds freertos_tick_interval{base::Hz{configTICK_RATE_HZ}};
	int64_t tick_count = static_cast<int64_t>(timeout / freertos_tick_interval);
	if (tick_count >= static_cast<int64_t>(portMAX_DELAY))
	{
		throw std::invalid_argument{CODE_POS_STR + "等待时间过长。"};
	}

	bool result = xSemaphoreTake(_semaphore, tick_count);
	if (_disposed)
	{
		throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
	}

	return result;
}

std::shared_ptr<base::ISemaphore> base::CreateISemaphore(int32_t initial_count)
{
	return std::shared_ptr<base::ISemaphore>{new task::DisposableSemaphore{initial_count}};
}
