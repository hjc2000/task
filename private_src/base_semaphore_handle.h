#pragma once
#include "base/string/define.h"
#include "base/task/base_semaphore_handle.h"
#include "base/unit/Hz.h"
#include "base/unit/Second.h"
#include "FreeRTOS.h" // IWYU pragma: keep
#include "semphr.h"

class base::task::base_semaphore_handle
{
private:
	SemaphoreHandle_t _semaphore{};
	BaseType_t _higher_priority_task_woken = pdFALSE;

public:
	///
	/// @brief 构造函数。
	///
	/// @param initial_count 初始计数。即构造后，
	/// 	@note 不释放就能允许 initial_count 次获取操作不被阻塞。
	///
	base_semaphore_handle(int32_t initial_count)
	{
		_semaphore = xSemaphoreCreateCounting(60000, initial_count);
	}

	///
	/// @brief 释放信号量。
	///
	/// @param count 要释放的数量。
	///
	void Release(int32_t count)
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

	///
	/// @brief 在中断中释放信号量。
	///
	/// @param count
	///
	void ReleaseFromISR(int32_t count)
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

	///
	/// @brief 获取信号量。无限等待，永不超时。
	///
	///
	void Acquire()
	{
		xSemaphoreTake(_semaphore, portMAX_DELAY);
	}

	///
	/// @brief 尝试获取信号量。超时后会失败，并返回 false，成功获取信号量后将返回 true.
	///
	/// @param timeout 超时时间。
	///
	/// @return true
	/// @return false
	///
	bool TryAcquire(base::unit::Second const &timeout)
	{
		if (timeout < base::unit::Second{0})
		{
			throw std::invalid_argument{CODE_POS_STR + "超时时间不能 <= 0."};
		}

		base::unit::Second freertos_tick_interval{base::unit::Hz{configTICK_RATE_HZ}};
		int64_t tick_count = static_cast<int64_t>(timeout / freertos_tick_interval);
		if (tick_count >= static_cast<int64_t>(portMAX_DELAY))
		{
			throw std::invalid_argument{CODE_POS_STR + "等待时间过长。"};
		}

		bool result = xSemaphoreTake(_semaphore, tick_count);
		return result;
	}
};
