#pragma once
#include "FreeRTOS.h"
#include "semphr.h"
#include <bsp-interface/task/IBinarySemaphore.h>
#include <stdexcept>

namespace task
{
	/// @brief 包装 freertos 的二进制信号量。
	class BinarySemaphore :
		public bsp::IBinarySemaphore
	{
	private:
		SemaphoreHandle_t handle;
		BaseType_t _higher_priority_task_woken = pdFALSE;

	public:
		BinarySemaphore();
		~BinarySemaphore();

		/// @brief 释放信号量。
		void Release() override;

		/// @brief 在中断中释放信号量。
		void ReleaseFromISR() override;

		/// @brief 获取信号量。无限等待，永不超时。
		void Acquire() override;

		/// @brief 尝试获取信号量。超时后会失败，并返回 false，成功获取信号量后将
		/// 返回 true.
		/// @param timeout 超时时间。
		/// @return
		bool TryAcquire(base::Seconds const &timeout) override;

		/// @brief 尝试获取信号量。超时后会失败，并返回 false，成功获取信号量后将
		/// 返回 true.
		/// @param ticks 超时时间，单位：freertos 的 tick 数。
		/// @return
		bool TryAcquire(TickType_t ticks = 0);
	};
} // namespace task
