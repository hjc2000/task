#include <bsp-interface/di/delayer.h>
#include <TaskDelayer.h>

bsp::IDelayer &bsp::di::Delayer()
{
	return task::TaskDelayer::Instance();
}
