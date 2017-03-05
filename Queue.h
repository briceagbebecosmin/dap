#ifndef _QUEUE_H_
#define _QUEUE_H_



typedef struct
{
    Job asJobs[JOBS];
    u16 uwHead;
    u16 uwTail;
}WorkQueue;

void wq_vEnqueue(WorkQueue *psQueue, const Job *cpsJob);

Job* wq_vDequeue(WorkQueue *psQueue);

#endif