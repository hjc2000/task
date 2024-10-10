#pragma once
#include <base/define.h>
#include <bsp-interface/task/ITask.h>
#include <FreeRTOS.h>
#include <functional>
#include <memory>
#include <stdint.h>
#include <task.h>

namespace task
{
    class Task : public bsp::ITask
    {
    private:
        std::function<void()> _func;
        TaskHandle_t _handle;

    public:
        /// @brief 创建一个任务以执行 func。
        /// @param func
        ///
        /// @param stack_depth 任务的栈大小。
        /// @note 注意，单位不是字节，而是字。32 位 CPU 一个字是 4 字节。
        ///
        /// @return
        static_function std::shared_ptr<task::Task> Create(std::function<void()> func, uint16_t stack_depth);
    };
} // namespace task
