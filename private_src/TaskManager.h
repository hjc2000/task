#pragma once
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/task/ITaskManager.h>
#include <task/Task.h>

namespace bsp
{
    class TaskManager :
        public bsp::ITaskManager
    {
    private:
        TaskManager() = default;

    public:
        static_function TaskManager &Instance()
        {
            class Getter :
                public base::SingletonGetter<TaskManager>
            {
            public:
                std::unique_ptr<TaskManager> Create() override
                {
                    return std::unique_ptr<TaskManager>{new TaskManager{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }

        /// @brief 启动运行一个任务
        ///
        /// @param func 任务函数
        ///
        /// @param stack_size 任务栈大小。单位：字。
        /// @note 对于 32 位宽的 CPU ，一个字是 32 位，即 4 个字节。
        std::shared_ptr<bsp::ITask> Create(std::function<void()> func, uint32_t stack_size) override
        {
            return task::Task::Create(func, stack_size);
        }
    };
} // namespace bsp
