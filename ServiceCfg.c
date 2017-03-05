#include "Types.h"

/*Include here the library from the MCU */

#include "ServiceCfg.h"



const RequestHandler acsHandlers[SERVICES] =
{
		{
				.pfctHandler =  null,
				.ubCmd       =  SERVICE_GETADC_CMD
		},
		{
				.pfctHandler = null,
				.ubCmd       = SERVICE_SETPWM_CMD
		}
};

const Sender csSender = 
{
	.pfctSendRaw = null
};