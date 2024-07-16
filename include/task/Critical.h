#pragma once
#include <FreeRTOS.h>
#include <functional>
#include <task.h>

namespace task
{
	/// @brief 用于进出临界区
	class Critical
	{
	private:
		Critical() = delete;

		static uint32_t _isr_enter_ret;

	public:
		static void Enter()
		{
			taskENTER_CRITICAL();
		}

		static void EnterFromISR()
		{
			_isr_enter_ret = taskENTER_CRITICAL_FROM_ISR();
		}

		static void Exit()
		{
			taskEXIT_CRITICAL();
		}

		static void ExitFromISR()
		{
			taskEXIT_CRITICAL_FROM_ISR(_isr_enter_ret);
		}

		/// @brief 在非中断中执行临界区任务
		/// @param f
		static void Run(std::function<void()> f)
		{
			Enter();
			f();
			Exit();
		}

		static void RunFromISR(std::function<void()> f)
		{
			EnterFromISR();
			f();
			ExitFromISR();
		}
	};
}
