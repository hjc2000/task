#include <bsp-interface/di/task.h>
#include <TaskManager.h>

bsp::ITaskManager &DI_TaskManager()
{
	return bsp::TaskManager::Instance();
}

/// @brief 暂停所有调度。
void DI_SuspendAllTask()
{
	vTaskSuspendAll();
}

/// @brief 恢复所有调度。
/// @note 如果调度器没有启动，本函数不会启动调度器。
void DI_ResumeAllTask()
{
	xTaskResumeAll();
}
