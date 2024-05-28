#include"Mutex.h"

task::Mutex::Mutex()
{
	_freertos_mutex = xSemaphoreCreateMutex();
	if (_freertos_mutex == nullptr)
	{
		throw std::runtime_error{ "Mutex 构造失败" };
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
