#pragma once
#include <bsp-interface/IDelayer.h>

namespace task
{
	class TaskDelayer
		: public bsp::IDelayer
	{
	private:
		TaskDelayer() = default;

	public:
		static TaskDelayer &Instance()
		{
			static TaskDelayer o;
			return o;
		}

		void Delay(std::chrono::microseconds microseconds);
		void Delay(std::chrono::milliseconds milliseconds);
		void Delay(std::chrono::seconds seconds);
	};
}
