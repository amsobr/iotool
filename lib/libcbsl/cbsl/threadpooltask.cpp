#include "threadpool.h"
#include "threadpooltask.h"

namespace cbsl {
void ThreadPoolTask::requestWork()
{
    myPool->enqueueTask(this);    
}

} /* namespace Wcommon */
