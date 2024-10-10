#include "FreeTask.h"
#include <stdexcept>

std::shared_ptr<task::FreeTask>
task::FreeTask::Create(std::function<void()> func, uint16_t stack_depth)
{
    if (func == nullptr)
    {
        throw std::invalid_argument{"func 不能为 nullptr"};
    }

    std::shared_ptr<task::FreeTask> task{new task::FreeTask{}};
    task->_func = func;

    /* 将智能指针放入 map 中，key 为裸指针，value 为智能指针。
     * 这么做是为了让本函数的调用者不用接收返回值并时刻保持着返回值的存在。
     * 智能指针放入 map 后，即使本函数返回值被丢弃，task::Task 对象也不会被回收，
     * 从而避免了 freertos 执行任务到一半，lambda 表达式对象被销毁了。
     */
    std::pair<task::FreeTask *, std::shared_ptr<task::FreeTask>> pair{task.get(), task};
    _task_map.insert(pair);

    auto f = [](void *param)
    {
        task::FreeTask *task = static_cast<task::FreeTask *>(param);

        try
        {
            task->_func();
        }
        catch (...)
        {
        }

        // 任务结束后需要将 task 从 map 中移除，保证指向的 task::Task 对象能够析构。
        task::FreeTask::_task_map.erase(task);

        // 任务要退出必须删除自身，否则会内存泄漏
        vTaskDelete(nullptr);
    };

    xTaskCreate(f,
                "Task",
                stack_depth,
                task.get(),
                1,
                &task->_handle);

    // 本类对象内的字段借由拷贝构造函数拷贝给接收返回值的对象
    return task;
}
