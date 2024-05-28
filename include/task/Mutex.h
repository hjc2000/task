#pragma once
#include<FreeRTOS.h>
#include<semphr.h>
#include<stdexcept>

namespace task
{
	class Mutex
	{
	private:
		SemaphoreHandle_t _freertos_mutex;

	public:
		Mutex();
		~Mutex();

		bool Lock(TickType_t waitTime = portMAX_DELAY)
		{
			return xSemaphoreTake(_freertos_mutex, waitTime) == pdTRUE;
		}

		void Unlock()
		{
			xSemaphoreGive(_freertos_mutex);
		}
	};

	class MutexLockGuard
	{
	private:
		Mutex &_mutex;

	public:
		MutexLockGuard(Mutex &m) :
			_mutex(m)
		{
			bool lock_result = _mutex.Lock();
			if (!lock_result)
			{
				throw std::runtime_error{ "互斥锁加锁失败" };
			}
		}

		~MutexLockGuard()
		{
			_mutex.Unlock();
		}
	};
}
