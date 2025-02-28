#include <base/task/IMutex.h>
#include <Mutex.h>

/**
 * @brief 创建一个互斥量。
 *
 * @return std::shared_ptr<base::IMutex>
 */
std::shared_ptr<base::IMutex> base::di::CreateIMutex()
{
	return std::shared_ptr<base::IMutex>{new task::Mutex{}};
}
