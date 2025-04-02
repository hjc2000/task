#include "BinarySemaphore.h"
#include "base/string/define.h"
#include "base/unit/Hz.h"
#include <stdexcept>

task::BinarySemaphore::BinarySemaphore(bool initial_release)
{
	handle = xSemaphoreCreateBinary();
	if (handle == nullptr)
	{
		throw std::runtime_error{"创建信号量失败"};
	}

	if (initial_release)
	{
		Release();
	}
}

task::BinarySemaphore::~BinarySemaphore()
{
	vSemaphoreDelete(handle);
}

void task::BinarySemaphore::Release()
{
	xSemaphoreGive(handle);
}

void task::BinarySemaphore::ReleaseFromISR()
{
	xSemaphoreGiveFromISR(handle, &_higher_priority_task_woken);
}

void task::BinarySemaphore::Acquire()
{
	bool ret = xSemaphoreTake(handle, portMAX_DELAY);
	if (!ret)
	{
		throw std::runtime_error{"获取信号量失败"};
	}
}

bool task::BinarySemaphore::TryAcquire(base::Seconds const &timeout)
{
	if (timeout < base::Seconds{0})
	{
		throw std::invalid_argument{CODE_POS_STR + "超时时间不能 <= 0."};
	}

	base::Seconds tick_interval{base::Hz{configTICK_RATE_HZ}};
	int64_t tick_count = static_cast<int64_t>(timeout / tick_interval);

	// 因为 freertos 不支持传入很大的整型超时时间，所以分成几次等待。
	int64_t remain_tick_count = tick_count;
	while (remain_tick_count > INT16_MAX)
	{
		// 要等待的时间过长，在循环里慢慢等待，每次循环等待一个 INT16_MAX.
		bool result = TryAcquire(INT16_MAX);
		if (result)
		{
			// 成功获取信号量，提前结束。
			return true;
		}

		remain_tick_count -= INT16_MAX;
	}

	// 经过前面的循环的消磨，remain_tick_count 变得很小了，小于 INT16_MAX 了。
	return TryAcquire(remain_tick_count);
}

bool task::BinarySemaphore::TryAcquire(TickType_t ticks)
{
	return xSemaphoreTake(handle, ticks) == pdTRUE;
}

std::shared_ptr<base::IBinarySemaphore> base::CreateIBinarySemaphore(bool initial_release)
{
	return std::shared_ptr<base::IBinarySemaphore>{new ::task::BinarySemaphore{initial_release}};
}
