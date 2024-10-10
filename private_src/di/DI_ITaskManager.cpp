#include <bsp-interface/di/task.h>
#include <TaskManager.h>

bsp::ITaskManager &DI_TaskManager()
{
    return bsp::TaskManager::Instance();
}
