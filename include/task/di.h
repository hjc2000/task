#pragma once
#include <bsp-interface/ISysTick.h>

#pragma region 声明，没有实现
bsp::ISysTick &DI_SysTick();
#pragma endregion

#pragma region 提供实现
bsp::IDelayer &DI_Delayer();
#pragma endregion
