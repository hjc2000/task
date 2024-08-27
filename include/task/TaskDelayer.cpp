#include "TaskDelayer.h"
#include <base/Initializer.h>
#include <bsp-interface/di.h>
#include <FreeRTOS.h>
#include <task.h>

/// @brief 注册初始化函数
static base::Initializer _task_delayer_initializer{
    []()
    {
        task::TaskDelayer::Instance();
    }};

void task::TaskDelayer::Delay(std::chrono::microseconds microseconds)
{
    // 大于 1000 的微秒转换为毫秒，利用 Delay(std::chrono::milliseconds milliseconds) 重载。
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(microseconds);

    if (ms.count() > 0)
    {
        Delay(std::chrono::milliseconds{ms});
    }

    // 剩余的小于 1000 部分的微秒
    microseconds -= ms;
    DI_SysTick().Delay(microseconds);
}

void task::TaskDelayer::Delay(std::chrono::milliseconds milliseconds)
{
    // 如果调度器不在运行，则使用 SysTickClock::Delay
    if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
    {
        DI_SysTick().Delay(milliseconds);
        return;
    }

    /*
     * FreeRTOSConfig.h 中的 configTICK_RATE_HZ 定义了 freertos 的 tick 频率。
     * 注意，这个频率不是 SysTickClock 硬件时钟的频率。
     *
     * freertos tick 周期
     *	T = 1 / configTICK_RATE_HZ
     * 单位为秒
     *
     * 要延时的 tick 数为
     *	tick = milliseconds / 1000 / T
     *	tick = milliseconds / 1000 * configTICK_RATE_HZ
     * 为了防止 milliseconds / 1000 出现截断误差，调整为
     *	tick = milliseconds * configTICK_RATE_HZ / 1000
     *
     * 记
     *	count = milliseconds * configTICK_RATE_HZ
     * 求出
     *	base = count / 1000
     * 这部分可以用 freertos 延时。
     *	mod = count % 1000
     * 这部分利用 freertos 延时的话会被截断，所以用 SysTickClock::Delay 延时。
     */

    int64_t count = milliseconds.count() * configTICK_RATE_HZ;
    int64_t base = count / 1000;
    int64_t mod = count % 1000;
    if (base > 0)
    {
        vTaskDelay(base);
    }

    if (mod > 0)
    {
        DI_SysTick().Delay(std::chrono::milliseconds{mod / configTICK_RATE_HZ});
    }
}

void task::TaskDelayer::Delay(std::chrono::seconds seconds)
{
    Delay(std::chrono::milliseconds{seconds});
}
