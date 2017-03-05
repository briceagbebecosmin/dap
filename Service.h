#ifndef _SERVICE_H_
#define _SERVICE_H_



typedef enum
{
    eACQUIRED = 0,
    eSERVING  = 1,
    eDONE     = 2
}JobState;

typedef struct
{
    JobState eJobState;
    Header   sHeader;
    u8       ubJobDataOffset;
}Job;

#define JOBS     ((u8)256)
#define SERVICES ((u8)2)

#define SERVICE_GETADC_CMD ((u8)0x00)
#define SERVICE_SETPWM_CMD ((u8)0x01)
#define SERVICE_SETDAC_CMD ((u8)0x02)
#define SERVICE_GETI2C_CMD ((u8)0x03)
#define SERVICE_GETSPI_CMD ((u8)0x04)
#define SERVICE_FWD_CMD    ((u8)0x05)

typedef struct
{
	void (*pfctHandler)(u8 *pubArgs);
	u8 ubCmd;

}RequestHandler;




/*! \brief 
 *   Initialize the service module
 */
void serv_vInit(void);


/*! \brief 
 *   Execute the main thread of processing incoming frames
 */
void serv_vRunService(const u8* cpubFrame);



#endif
