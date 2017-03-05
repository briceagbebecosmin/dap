#include "Types.h"
#include "Util.h"
#include "Protocol.h"
#include "ServiceCfg.h"
#include "Queue.h"
#include "Service.h"


static void serv_vGetAdcRaw(u8 *pubData);

/*! \brief
 *   Dispatching a job took from the working queue
 */
static void serv_vDispatch(Job *psJob);

static void serv_vExec(Job* psJob);

static void serv_vError(const u8 cubError,const Header* cpsHeader);

static void serv_vSavejob(Job *psJob);

static void serv_vPutResponse(Job* psJob);

static void serv_vRunJobs(void);


static u8 serv_aubJobMemory[1024];

static u8 serv_aubOutputBuffer[1024];

static u16  serv_uwJobs;

static WorkQueue serv_sQueue;

static u16 serv_uwDataSize;

void serv_vInit(void)
{
	serv_uwJobs     = (u16)0;
	serv_uwDataSize = (u16)0;
}

static void serv_vDispatch(Job *psJob)
{
	psJob->eJobState = eSERVING;

	serv_vExec(psJob);

	psJob->eJobState = eDONE;

	serv_uwJobs--;
}

void serv_vRunService(const u8 *cpubFrame)
{
	Header sHeader;
	Job    sJob;

	sHeader.ubFID    = *(cpubFrame + FID_OFFSET);
	sHeader.ubPort   = *(cpubFrame + PORT_OFFSET);
	sHeader.ulDevice = (u32)((u32)*(cpubFrame + DEVICE_OFFSET)
			| ((u32)*(cpubFrame + DEVICE_OFFSET + 1) << 8)
			| ((u32)*(cpubFrame + DEVICE_OFFSET + 2) << 16)
			| ((u32)*(cpubFrame + DEVICE_OFFSET + 3) << 24));
	sHeader.ubCommand = *(cpubFrame + COMMAND_OFFSET);
	sHeader.uwTimeStamp = (u16)(*(cpubFrame + TIMESTAMP_OFFSET)
			|((u16)*(cpubFrame + TIMESTAMP_OFFSET + 1))<< 8);
	sHeader.ulDS = (u32)(*(cpubFrame + DLC_OFFSET)
			| (((u32)*(cpubFrame + DLC_OFFSET + 1))  << 8)
			| (((u32)*(cpubFrame + DLC_OFFSET + 2)) << 16)
			| (((u32)*(cpubFrame + DLC_OFFSET + 3)) << 24));

	if(serv_uwJobs != (JOBS - ((u16)1)))
	{
		util_vMemCopy(&sHeader,&sJob.sHeader,sizeof(Header));
		sJob.ubJobDataOffset = (serv_uwJobs * (u16)4);
		serv_vSavejob(&sJob);

	}else
	{
		/* There is no more room left for data */
		serv_vError(FRAME_SERVICE_BUSY,&sHeader);
	}
}

static void serv_vExec(Job* psJob)
{
	const RequestHandler* cpsHandler = &acsHandlers[0];

	if(psJob->sHeader.ubCommand < (SERVICES - (u8)1))
	{
		cpsHandler[psJob->sHeader.ubCommand].pfctHandler(&serv_aubJobMemory[psJob->ubJobDataOffset],&serv_uwDataSize);

        psJob->sHeader.ulDS = (u32)serv_uwDataSize;
		serv_vPutResponse(psJob);

	}else
	{
		psJob->eJobState = eDONE;
		serv_vError(FRAME_CMD_ERROR,&psJob->sHeader);
	}
}

static void serv_vError(const u8 cubError,const Header* cpsHeader)
{
	Job sJob;

	util_vMemCopy(&sJob.sHeader,cpsHeader,sizeof(Header));

	sJob.sHeader.ubFID   = cubError;
	sJob.ubJobDataOffset = (u8)0;
	sJob.eJobState       = eDONE;

	serv_vPutResponse(&sJob);
}

static void serv_vSavejob(Job *psJob)
{
	psJob->eJobState = eACQUIRED;
	serv_uwJobs++;

	wq_vEnqueue(&serv_sQueue,psJob);
}

static void serv_vPutResponse(Job* psJob)
{
	Job psLocalJob;
	const Sender *psSender = &csSender;
	u32 ulSizeOfData = (u32)0;

	util_vMemCopy(&psLocalJob,psJob,sizeof(Job));

	util_vMemCopy(&serv_aubOutputBuffer[0],(u8*)&psLocalJob.sHeader,sizeof(Header));

	util_vMemCopy(&serv_aubOutputBuffer[sizeof(Header)],&serv_aubJobMemory[psLocalJob.ubJobDataOffset],psLocalJob.sHeader.ulDS);

	serv_aubJobMemory[(u32)sizeof(Header) + psLocalJob.sHeader.ulDS] = 0x3B; /* EOF = ';'*/

	ulSizeOfData = ((u32)sizeof(Header) + psLocalJob.sHeader.ulDS + (u32)1);

	psSender->pfctSendRaw(&serv_aubOutputBuffer[0],ulSizeOfData);
}
 
/* Main thread to execute the jobs put in queue */
static void serv_vRunJobs(void)
{
	Job* psJob;

	while((psJob = wq_vDequeue(&serv_sQueue)) != null)
	{
		serv_vDispatch(psJob);
	} 
}




