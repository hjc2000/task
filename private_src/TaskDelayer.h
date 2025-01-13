#pragma once
#include <base/define.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/timer/IDelayer.h>

namespace task
{
	class TaskDelayer :
		public bsp::IDelayer
	{
	private:
		TaskDelayer() = default;

	public:
		static_function TaskDelayer &Instance();

		void Delay(std::chrono::microseconds microseconds);
		void Delay(std::chrono::milliseconds milliseconds);
		void Delay(std::chrono::seconds seconds);
	};
} // namespace task
