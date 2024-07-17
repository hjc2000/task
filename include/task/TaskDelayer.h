#pragma once
#include <bsp-interface/IDelayer.h>

namespace task
{
	class TaskDelayer
		: public bsp::IDelayer
	{
	public:
		void Delay(std::chrono::microseconds microseconds);
		void Delay(std::chrono::milliseconds milliseconds);
		void Delay(std::chrono::seconds seconds);
	};
}
