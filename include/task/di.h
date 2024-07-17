#pragma once
#include <bsp-interface/ISysTick.h>

/// @brief 这里都是本库的依赖项，采用声明函数，让其返回接口实例的引用的方式。这是一种依赖注入模式，
/// 库的使用者需要实现这些函数，否则将引发链接错误。
/// @note 这里的函数都以 DI_ 开头。是 dependency injection 的缩写，表示依赖注入。
namespace task
{
	bsp::ISysTick &DI_SysTick();
}
