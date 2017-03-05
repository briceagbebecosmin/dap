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






/*! \brief 
 *   Initialize the service module
 */
void serv_vInit(void);


/*! \brief 
 *   Execute the main thread of processing incoming frames
 */
void serv_vRunService(const u8* cpubFrame);



#endif
