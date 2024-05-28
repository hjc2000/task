#pragma once
#include<FreeRTOS.h>
#include<functional>
#include<memory>
#include<stdint.h>
#include<task.h>

namespace task
{
	class Task
	{
	private:
		std::function<void()> _func;
		TaskHandle_t _handle;

	public:
		/// <summary>
		///		创建一个任务以执行 func。
		///		* 任务的栈大小使用默认值 DefaultStackDepth()
		/// </summary>
		/// <param name="func"></param>
		/// <param name="stack_depth">
		///		任务的栈大小。
		///		* 注意，单位不是字节，而是字。32 位 CPU 一个字是 4 字节。
		/// </param>
		/// <returns></returns>
		static std::shared_ptr<task::Task> Create(std::function<void()> func, uint16_t stack_depth);
	};
}
