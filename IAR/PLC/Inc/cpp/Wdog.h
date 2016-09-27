#pragma once

#include "RGsystem.h"

#define wdClear()	IWDG->KR=0xAAAA

namespace RGsystem
{
	void WDinit(void);
}
