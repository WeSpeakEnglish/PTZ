#include "Wdog.h"

namespace RGsystem
{
	void WDinit(void)
	{
		DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;// Debug WD stop
		IWDG->KR=0x5555;// Enter protection
		IWDG->PR=0x02;// Precaler = 16
		IWDG->RLR=0x7D;// Reload value
		IWDG->KR=0xAAAA;// Refresh to RLR val
		IWDG->KR=0xCCCC;// Start timer
	}
}
