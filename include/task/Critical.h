#pragma once
#include <base/define.h>
#include <FreeRTOS.h>
#include <functional>
#include <task.h>

namespace task
{
    /// @brief 用于进出临界区
    class Critical
    {
    private:
        Critical() = delete;

        static_field uint32_t _isr_enter_ret;

    public:
        static_function void Enter()
        {
            taskENTER_CRITICAL();
        }

        static_function void EnterFromISR()
        {
            _isr_enter_ret = taskENTER_CRITICAL_FROM_ISR();
        }

        static_function void Exit()
        {
            taskEXIT_CRITICAL();
        }

        static_function void ExitFromISR()
        {
            taskEXIT_CRITICAL_FROM_ISR(_isr_enter_ret);
        }

        /// @brief 在非中断中执行临界区任务
        /// @param f
        static_function void Run(std::function<void()> f)
        {
            Enter();
            f();
            Exit();
        }

        static_function void RunFromISR(std::function<void()> f)
        {
            EnterFromISR();
            f();
            ExitFromISR();
        }
    };
} // namespace task
