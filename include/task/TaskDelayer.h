#pragma once
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/IDelayer.h>

namespace task
{
    class TaskDelayer : public bsp::IDelayer
    {
    private:
        TaskDelayer() = default;

    public:
        static TaskDelayer &Instance()
        {
            class Getter : public base::SingletonGetter<TaskDelayer>
            {
            public:
                std::unique_ptr<TaskDelayer> Create() override
                {
                    return std::unique_ptr<TaskDelayer>{new TaskDelayer{}};
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

        void Delay(std::chrono::microseconds microseconds);
        void Delay(std::chrono::milliseconds milliseconds);
        void Delay(std::chrono::seconds seconds);
    };
} // namespace task
