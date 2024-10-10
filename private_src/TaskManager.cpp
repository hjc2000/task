#include "TaskManager.h"

bsp::TaskManager &bsp::TaskManager::Instance()
{
    class Getter :
        public base::SingletonGetter<TaskManager>
    {
    public:
        std::unique_ptr<TaskManager> Create() override
        {
            return std::unique_ptr<TaskManager>{new TaskManager{}};
        }

        void Lock() override
        {
            DI_InterruptSwitch().DisableGlobalInterrupt();
        }

        void Unlock() override
        {
            DI_InterruptSwitch().EnableGlobalInterrupt();
        }
    };

    Getter g;
    return g.Instance();
}
