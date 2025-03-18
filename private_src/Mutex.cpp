#include "Mutex.h"

task::Mutex::Mutex()
{
	_freertos_mutex = xSemaphoreCreateMutex();
	if (_freertos_mutex == nullptr)
	{
		throw std::runtime_error{"Mutex 构造失败"};
	}
}

task::Mutex::~Mutex()
{
	if (_freertos_mutex == nullptr)
	{
		return;
	}

	Unlock();
	vSemaphoreDelete(_freertos_mutex);
}

void task::Mutex::Lock()
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

void task::Mutex::Unlock()
{
	xSemaphoreGive(_freertos_mutex);
}
