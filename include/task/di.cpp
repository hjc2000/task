#include "di.h"
#include <task/TaskDelayer.h>

bsp::IDelayer &DI_Delayer()
{
	return task::TaskDelayer::Instance();
}
