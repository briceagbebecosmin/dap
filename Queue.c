#include "Types.h"
#include "ServiceCfg.h"
#include "Util.h"
#include "Queue.h"


void wq_vEnqueue(WorkQueue *psQueue, const Job *cpsJob)
{
    if(psQueue->uwTail == (JOBS - (u16)1))
    {
        psQueue->uwTail = (u16)0;
    }else
    {
        ++psQueue->uwTail;  
    }

    if(psQueue->uwHead == psQueue->uwTail)
    {
        /* Queue is full */
        return;
    }

    util_vMemCopy(&psQueue->asJobs[psQueue->uwTail],cpsJob,sizeof(Job));
}

Job* wq_vDequeue(WorkQueue *psQueue)
{
    if(psQueue->uwHead == psQueue->uwTail)
    {
        /* Queue is empty */
        return null;
    }

    if(psQueue->uwHead == (JOBS - (u16)1))
    {
        psQueue->uwHead = (u16)0;
    }else
    {
        ++psQueue->uwHead;
    }

    return (&psQueue->asJobs[psQueue->uwHead]);
}