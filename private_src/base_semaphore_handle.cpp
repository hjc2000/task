#include "base_semaphore_handle.h"

std::shared_ptr<base::task::base_semaphore_handle> base::task::create_base_semaphore(int32_t initial_count)
{
	return std::shared_ptr<base::task::base_semaphore_handle>{new base::task::base_semaphore_handle{initial_count}};
}

void base::task::release(base::task::base_semaphore_handle &h, int32_t count)
{
	h.Release(count);
}

void base::task::acquire(base::task::base_semaphore_handle &h)
{
	h.Acquire();
}

bool base::task::try_acquire(base::task::base_semaphore_handle &h, base::unit::Second const &timeout)
{
	return h.TryAcquire(timeout);
}
