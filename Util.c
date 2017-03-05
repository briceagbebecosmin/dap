#include "Types.h"
#include "Util.h"

void util_vMemSet(void *pvDest, const u8 cubFill, const u32 culSize)
{
    u32 ulSize = culSize;
    u8 *pubDest = (u8 *)pvDest;

    while (ulSize != ((u8)0))
    {
        *pubDest++ = cubFill;
        ulSize--;
    }
}

void util_vMemCopy(void *pvDest, const void *cpvSrc, const u32 culSize)
{
    u32 ulSize = culSize;
    u8 *pubDest = (u8 *)pvDest;
    const u8 *cpubSrc = (u8 *)cpvSrc;

    while (ulSize != (u8)0)
    {
        *pubDest++ = *cpubSrc++;
        ulSize--;
    }
}
