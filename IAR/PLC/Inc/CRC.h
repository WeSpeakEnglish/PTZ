#ifndef CRC_H
#define CRC_H
#include "variables.h"

uint8_t getCRCbusy();
void setCRCbusy(uint8_t val);
void CRCinit();
void CRCreset();
uint32_t getCRC();
void setCRC(uint32_t value);
uint32_t CRCcalculateBuf(uint8_t *buf, uint16_t bufLen);
void CRCnext(uint32_t data);
#endif
        