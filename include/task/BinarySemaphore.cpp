#include"BinarySemaphore.h"

using namespace task;

BinarySemaphore::BinarySemaphore()
{
	handle = xSemaphoreCreateBinary();
	if (handle == nullptr)
	{
		throw std::runtime_error{ "创建信号量失败" };
	}
}

BinarySemaphore::~BinarySemaphore()
{
	vSemaphoreDelete(handle);
}

void BinarySemaphore::Release()
{
	xSemaphoreGive(handle);
}

void BinarySemaphore::ReleaseFromISR()
{
	xSemaphoreGiveFromISR(handle, &_higher_priority_task_woken);
}

void BinarySemaphore::Acquire()
{
	bool ret = xSemaphoreTake(handle, portMAX_DELAY);
	if (!ret)
	{
		throw std::runtime_error{ "获取信号量失败" };
	}
}

bool BinarySemaphore::TryAcquire(TickType_t ticks)
{
	return xSemaphoreTake(handle, ticks) == pdTRUE;
}
