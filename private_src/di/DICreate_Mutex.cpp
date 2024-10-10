#include <bsp-interface/di/task.h>
#include <Mutex.h>

std::shared_ptr<bsp::IMutex> DICreate_Mutex()
{
    return std::shared_ptr<bsp::IMutex>{new task::Mutex{}};
}
