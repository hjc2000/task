#include "binary_semaphore_handle.h"

std::shared_ptr<base::task::binary_semaphore_handle> base::task::create_binary_semaphore(bool initial_release)
{
	return std::shared_ptr<base::task::binary_semaphore_handle>{new base::task::binary_semaphore_handle{initial_release}};
}

void base::task::release(base::task::binary_semaphore_handle &h)
{
	h.Release();
}

void base::task::release_from_isr(base::task::binary_semaphore_handle &h)
{
	h.ReleaseFromISR();
}

void base::task::acquire(base::task::binary_semaphore_handle &h)
{
	h.Acquire();
}

bool base::task::try_acquire(base::task::binary_semaphore_handle &h, base::unit::Second const &timeout)
{
	return h.TryAcquire(timeout);
}
