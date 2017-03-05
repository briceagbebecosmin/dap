#include "Types.h"
#include "Util.h"
#include "Protocol.h"
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


static u8 serv_aubJobMemory[1024];

static u16  serv_uwJobs;

static const RequestHandler acsHandlers[SERVICES] =
{
		{
				.pfctHandler =  serv_vGetAdcRaw,
				.ubCmd       =  SERVICE_GETADC_CMD
		},
		{
				.pfctHandler = null,
				.ubCmd       = SERVICE_SETPWM_CMD
		}
};


void serv_vInit(void)
{
	serv_uwJobs = (u16)0;
}

static void serv_vDispatch(Job *psJob)
{
	psJob->eJobState = eSERVING;

	serv_vExec(psJob);

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
			| ((u32)*(cpubFrame + DEVICE_OFFSET + 2) << 24));
	sHeader.ubCommand = *(cpubFrame + COMMAND_OFFSET);
	sHeader.uwTimeStamp = (u16)(*(cpubFrame + TIMESTAMP_OFFSET)
			|((u16)*(cpubFrame + TIMESTAMP_OFFSET + 1))<< 8);
	sHeader.ulDS = (u32)(*(cpubFrame + DLC_OFFSET)
			| (((u32)*(cpubFrame + DLC_OFFSET + 1))  << 8)
			| (((u32)*(cpubFrame + DLC_OFFSET + 2)) << 16)
			| (((u32)*(cpubFrame + DLC_OFFSET + 3)) << 24));

	if(serv_uwJobs != (JOBS - ((u16)1))
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
		cpsHandler[psJob->sHeader.ubCommand].pfctHandler(&serv_aubJobMemory[psJob->ubJobDataOffset]);

		serv_vPutResponse(psJob);

		psJob->eJobState = eDONE;

	}else
	{
		psJob->eJobState = eDONE;
		serv_vError(FRAME_CMD_ERROR,&psJob->sHeader);
	}
}

static void serv_vError(const u8 cubError,const Header* cpsHeader)
{

}

static void serv_vGetAdcRaw(u8 *pubData)
{
	/* TODO Put the response data to the same location */
}

static void serv_vSavejob(Job *psJob)
{
	psJob->eJobState = eACQUIRED;
	serv_uwJobs++;
}

static void serv_vPutResponse(Job* psJob)
{
	Job psLocalJob;

	util_vMemCopy(psJob,&psLocalJob,sizeof(Job));

}






