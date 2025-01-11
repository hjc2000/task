#pragma once
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/systick.h>
#include <bsp-interface/task/ITaskManager.h>
#include <FreeTask.h>

namespace bsp
{
	class TaskManager :
		public bsp::ITaskManager
	{
	private:
		TaskManager() = default;

	public:
		static_function TaskManager &Instance();

		/// @brief 启动运行一个任务
		///
		/// @param func 任务函数
		///
		/// @param stack_size 任务栈大小。单位：字。
		/// @note 对于 32 位宽的 CPU ，一个字是 32 位，即 4 个字节。
		std::shared_ptr<bsp::ITask> Create(std::function<void()> func, uint32_t stack_size) override;

		/// @brief 启动调度。本函数会持续阻塞。
		void StartScheduler() override;

		/// @brief 暂停所有调度。
		virtual void SuspendAllTask() override;

		/// @brief 恢复所有调度。
		/// @note 如果调度器没有启动，本函数不会启动调度器。
		virtual void ResumeAllTask() override;
	};
} // namespace bsp
