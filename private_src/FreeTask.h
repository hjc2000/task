#pragma once
#include <base/define.h>
#include <bsp-interface/task/ITask.h>
#include <FreeRTOS.h>
#include <functional>
#include <map>
#include <memory>
#include <stdint.h>
#include <task.h>

namespace task
{
	/// @brief 任务
	class FreeTask :
		public bsp::ITask
	{
	private:
		std::function<void()> _func;
		TaskHandle_t _handle;
		inline static_field std::map<task::FreeTask *, std::shared_ptr<task::FreeTask>> _task_map{};

	public:
		/// @brief 创建一个任务并执行。
		///
		/// @param func 任务函数。
		///
		/// @param stack_depth 任务的栈大小。
		/// @note 注意，单位不是字节，而是字。32 位 CPU 一个字是 4 字节。
		///
		/// @return 返回创建的任务对象。可以不用接收返回值，这并不会导致 FreeTask 对象在本函数调用
		/// 结束后析构，因为本函数会将任务存放到内部 map 中，防止本函数返回后智能指针的终结器执行。
		static_function std::shared_ptr<task::FreeTask> Create(std::function<void()> func,
															   uint16_t stack_depth);
	};
} // namespace task
