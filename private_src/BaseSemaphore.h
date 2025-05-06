#pragma once
#include "base/task/IBaseSemaphore.h"
#include "FreeRTOS.h" // IWYU pragma: keep
#include "semphr.h"

namespace task
{
	class BaseSemaphore :
		public base::IBaseSemaphore
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
		BaseSemaphore(int32_t initial_count);

		///
		/// @brief 释放信号量。
		///
		/// @param count 要释放的数量。
		///
		virtual void Release(int32_t count) override;

		///
		/// @brief 在中断中释放信号量。
		///
		/// @param count
		///
		virtual void ReleaseFromISR(int32_t count) override;

		///
		/// @brief 获取信号量。无限等待，永不超时。
		///
		///
		virtual void Acquire() override;

		///
		/// @brief 尝试获取信号量。超时后会失败，并返回 false，成功获取信号量后将返回 true.
		///
		/// @param timeout 超时时间。
		///
		/// @return true
		/// @return false
		///
		virtual bool TryAcquire(base::Seconds const &timeout) override;
	};
} // namespace task
