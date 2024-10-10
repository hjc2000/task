#pragma once
#include "FreeRTOS.h"
#include "semphr.h"
#include <bsp-interface/task/IBinarySemaphore.h>
#include <stdexcept>

namespace task
{
    /// @brief 包装 freertos 的二进制信号量。
    class BinarySemaphore :
        public bsp::IBinarySemaphore
    {
    private:
        SemaphoreHandle_t handle;
        BaseType_t _higher_priority_task_woken = pdFALSE;

    public:
        BinarySemaphore();
        ~BinarySemaphore();

        void Release() override;
        void ReleaseFromISR() override;
        void Acquire() override;
        bool TryAcquire(TickType_t ticks = 0);
    };
} // namespace task
