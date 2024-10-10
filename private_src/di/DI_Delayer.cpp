#include <bsp-interface/di/delayer.h>
#include <TaskDelayer.h>

bsp::IDelayer &DI_Delayer()
{
    return task::TaskDelayer::Instance();
}
