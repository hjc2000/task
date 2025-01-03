#include <base/task/IMutex.h>
#include <Mutex.h>

namespace base
{
	namespace di
	{
		/// @brief 创建一个互斥量。
		/// @return
		std::shared_ptr<base::IMutex> CreateMutex()
		{
			return std::shared_ptr<base::IMutex>{new task::Mutex{}};
		}
	} // namespace di

} // namespace base
