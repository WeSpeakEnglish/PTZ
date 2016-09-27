#pragma once

#include "RGsystem.h"

namespace RGsystem
{
	bool getCRCbusy();
	void setCRCbusy(bool val);
	void CRCinit();
	void CRCreset();
	uint getCRC();
	void setCRC(uint value);
	uint CRCcalculateBuf(byte *buf, ushort bufLen);
	void CRCnext(uint data);
}
