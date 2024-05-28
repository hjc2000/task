#pragma once
#include"FreeRTOS.h"
#include"semphr.h"
#include<stdexcept>

namespace task
{
	/// <summary>
	///		包装 freertos 的二进制信号量。
	/// </summary>
	class BinarySemaphore
	{
	private:
		SemaphoreHandle_t handle;
		BaseType_t _higher_priority_task_woken = pdFALSE;

	public:
		BinarySemaphore();
		~BinarySemaphore();

		void Release();
		void ReleaseFromISR();
		void Acquire();
		bool TryAcquire(TickType_t ticks = 0);
	};
}
