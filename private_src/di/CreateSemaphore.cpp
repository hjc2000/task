#include <DisposableSemaphore.h>

namespace base
{
	namespace di
	{
		/// @brief 创建一个信号量。
		/// @param initial_count 信号量的初始计数。
		/// @return
		std::shared_ptr<base::ISemaphore> CreateSemaphore(int32_t initial_count)
		{
			return std::shared_ptr<base::ISemaphore>{new task::DisposableSemaphore{initial_count}};
		}
	} // namespace di
} // namespace base
