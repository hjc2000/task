#pragma once
#include <bsp-interface/task/IMutex.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <stdexcept>

namespace task
{
    class Mutex :
        public bsp::IMutex
    {
    private:
        SemaphoreHandle_t _freertos_mutex;

    public:
        Mutex();
        ~Mutex();

        void Lock() override;
        void Unlock() override;
    };

} // namespace task
