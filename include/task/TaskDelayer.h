#pragma once
#include <bsp-interface/IDelayer.h>
#include <task/dependencies.h>

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
