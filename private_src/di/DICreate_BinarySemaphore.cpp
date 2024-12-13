#include <BinarySemaphore.h>
#include <bsp-interface/di/task.h>

std::shared_ptr<bsp::IBinarySemaphore> DICreate_BinarySemaphore()
{
	return std::shared_ptr<bsp::IBinarySemaphore>{new task::BinarySemaphore{}};
}
