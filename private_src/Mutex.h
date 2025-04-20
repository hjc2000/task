#pragma once
#include "base/string/define.h"
#include "base/task/mutex_handle.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdexcept>

class base::task::mutex_handle
{
private:
	SemaphoreHandle_t _freertos_mutex;

public:
	mutex_handle()
	{
		_freertos_mutex = xSemaphoreCreateMutex();
		if (_freertos_mutex == nullptr)
		{
			throw std::runtime_error{CODE_POS_STR + "Mutex 构造失败"};
		}
	}

	~mutex_handle()
	{
		if (_freertos_mutex == nullptr)
		{
			return;
		}

		Unlock();
		vSemaphoreDelete(_freertos_mutex);
	}

	void Lock()
	{
		while (true)
		{
			TickType_t waitTime = portMAX_DELAY;
			bool result = xSemaphoreTake(_freertos_mutex, waitTime) == pdTRUE;
			if (result)
			{
				return;
			}
		}
	}

	void Unlock()
	{
		xSemaphoreGive(_freertos_mutex);
	}
};
