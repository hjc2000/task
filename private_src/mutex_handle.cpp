#include "mutex_handle.h"

std::shared_ptr<base::task::mutex_handle> base::task::create_mutex()
{
	return std::shared_ptr<base::task::mutex_handle>{new base::task::mutex_handle{}};
}

void base::task::lock(base::task::mutex_handle &h)
{
	h.Lock();
}

void base::task::unlock(base::task::mutex_handle &h)
{
	h.Unlock();
}
