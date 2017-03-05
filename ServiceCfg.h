#ifndef _SERVICE_CFG_H
#define _SERVICE_CFG_H_

#define JOBS     ((u8)256)
#define SERVICES ((u8)2)

#define SERVICE_GETADC_CMD   ((u8)0x00)
#define SERVICE_SETPWM_CMD   ((u8)0x01)
#define SERVICE_SETDAC_CMD   ((u8)0x02)
#define SERVICE_GETI2C_CMD   ((u8)0x03)
#define SERVICE_GETSPI_CMD   ((u8)0x04)
#define SERVICE_FWD_CMD      ((u8)0x05)
#define SERVICE_RESPONSE_CMD ((u8)0x06)


typedef struct
{
	void (*pfctHandler)(u8 *pubArgs,u16 *puwDataSize);
	u8 ubCmd;

}RequestHandler;

typedef struct
{
	void (*pfctSendRaw)(const u8 *cpubData, const u32 culSize);
}Sender;




extern const RequestHandler acsHandlers[SERVICES];

extern const Sender csSender;

#endif


