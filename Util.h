#ifndef _UTIL_H_
#define _UTIL_H_

/*! \brief 
 *   Fill the destination with cubFill data
 */
void util_vMemSet(void* pvDest,const u8 cubFill,const u32 culSize);

/*! \brief 
 *   Copy data from one side to another one 
 */
void util_vMemCopy(void* pvDest, const void* cpvSrc,const u32 culSize);


#endif