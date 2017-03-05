#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


typedef struct
{
	u32 ulDS;
	u32 ulDevice;
	u16 uwTimeStamp;
    u8  ubSOF;
    u8  ubFID;
    u8  ubPort;
    u8  ubCommand;
    u8  aubReserved[2];
}Header;

#define MAX_BUFFER_SIZE ((u16)2048)

#define SOF_OFFSET ((u8)0)
#define SOF_SIZE ((u8)1)

#define FID_OFFSET (((u8)1) + SOF_SIZE)
#define FID_SIZE ((u8)1)

#define PORT_OFFSET (((u8)2) + FID_SIZE)
#define PORT_SIZE ((u8)1)

#define DEVICE_OFFSET (((u8)3) + PORT_SIZE)
#define DEVICE_SIZE ((u8)4)

#define COMMAND_OFFSET (((u8)4) + DEVICE_SIZE)
#define COMMAND_SIZE ((u8)1)

#define TIMESTAMP_OFFSET (((u8)5) + COMMAND_SIZE)
#define TIMESTAMP_SIZE ((u8)2)

#define DLC_OFFSET (((u8)6) + TIMESTAMP_SIZE)
#define DLC_SIZE ((u8)4)

#define DATA_OFFSET (((u8)7) + DLC_SIZE)


#define FRAME_REQUEST      ((u8)1)
#define FRAME_ERROR        ((u8)2)
#define FRAME_MORE         ((u8)3)
#define FRAME_CMD_ERROR    ((u8)4)
#define FRAME_SERVICE_BUSY ((u8)5)


#endif
