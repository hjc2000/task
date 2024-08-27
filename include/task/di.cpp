#include <bsp-interface/di/delayer.h>
#include <task/TaskDelayer.h>

bsp::IDelayer &DI_Delayer()
{
    return task::TaskDelayer::Instance();
}
