#pragma once
#include "base/string/define.h"
#include "base/task/binary_semaphore_handle.h"
#include "base/unit/Hz.h"
#include "base/unit/Second.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdexcept>

///
/// @brief 包装 freertos 的二进制信号量。
///
class base::task::binary_semaphore_handle
{
private:
	SemaphoreHandle_t handle;
	BaseType_t _higher_priority_task_woken = pdFALSE;

public:
	binary_semaphore_handle(bool initial_release)
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

	~binary_semaphore_handle()
	{
		vSemaphoreDelete(handle);
	}

	///
	/// @brief 释放信号量。
	///
	void Release()
	{
		xSemaphoreGive(handle);
	}

	///
	/// @brief 在中断中释放信号量。
	///
	void ReleaseFromISR()
	{
		xSemaphoreGiveFromISR(handle, &_higher_priority_task_woken);
	}

	///
	/// @brief 获取信号量。无限等待，永不超时。
	///
	void Acquire()
	{
		bool ret = xSemaphoreTake(handle, portMAX_DELAY);
		if (!ret)
		{
			throw std::runtime_error{"获取信号量失败"};
		}
	}

	///
	/// @brief 尝试获取信号量。超时后会失败，并返回 false，成功获取信号量后将
	/// 返回 true.
	/// @param timeout 超时时间。
	/// @return
	///
	bool TryAcquire(base::unit::Second const &timeout)
	{
		if (timeout < base::unit::Second{0})
		{
			throw std::invalid_argument{CODE_POS_STR + "超时时间不能 <= 0."};
		}

		base::unit::Second tick_interval{base::unit::Hz{configTICK_RATE_HZ}};
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

	///
	/// @brief 尝试获取信号量。超时后会失败，并返回 false，成功获取信号量后将
	/// 返回 true.
	/// @param ticks 超时时间，单位：freertos 的 tick 数。
	/// @return
	///
	bool TryAcquire(TickType_t ticks = 0)
	{
		return xSemaphoreTake(handle, ticks) == pdTRUE;
	}
};
