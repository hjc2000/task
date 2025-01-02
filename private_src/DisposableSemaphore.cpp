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

	ReleaseAllAcquire();
}

void task::DisposableSemaphore::Release(int32_t count)
{
	if (count <= 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "count 不能 <=0."};
	}

	base::LockGuard g{*_lock};
	for (int64_t i = 0; i < count; i++)
	{
		xSemaphoreGive(_semaphore);
	}
}

void task::DisposableSemaphore::ReleaseAllAcquire()
{
	base::LockGuard g{*_lock};
	for (int64_t i = 0; i < _acquirer_count; i++)
	{
		xSemaphoreGive(_semaphore);
	}
}

void task::DisposableSemaphore::Acquire()
{
	{
		base::LockGuard g{*_lock};
		if (_disposed)
		{
			throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
		}

		_acquirer_count++;
	}

	xSemaphoreTake(_semaphore, portMAX_DELAY);

	{
		base::LockGuard g{*_lock};
		if (_disposed)
		{
			throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
		}

		_acquirer_count--;
	}
}

bool task::DisposableSemaphore::TryAcquire(base::Seconds const &timeout)
{
	{
		base::LockGuard g{*_lock};
		if (_disposed)
		{
			throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
		}

		_acquirer_count++;
	}

	base::Seconds freertos_tick_interval{base::Hz{configTICK_RATE_HZ}};
	int64_t tick_count = static_cast<int64_t>(timeout / freertos_tick_interval);
	if (tick_count >= static_cast<int64_t>(portMAX_DELAY))
	{
		throw std::invalid_argument{CODE_POS_STR + "等待时间过长。"};
	}

	bool result = xSemaphoreTake(_semaphore, tick_count);

	{
		base::LockGuard g{*_lock};
		if (_disposed)
		{
			throw std::runtime_error{CODE_POS_STR + "信号量已经释放，无法获取。"};
		}

		_acquirer_count--;
	}

	return result;
}
