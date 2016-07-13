#include "calculations.h"
#include "lcd.h"
#include <math.h>

const uint16_t SineTable[256] = {
	0x0000,	0x0192,	0x0324,	0x04b6,	0x0648,	0x07da,	0x096c,	0x0afe,
	0x0c90,	0x0e21,	0x0fb3,	0x1144,	0x12d5,	0x1466,	0x15f7,	0x1787,
	0x1918,	0x1aa8,	0x1c37,	0x1dc7,	0x1f56,	0x20e5,	0x2274,	0x2402,
	0x2590,	0x271e,	0x28ab,	0x2a38,	0x2bc4,	0x2d50,	0x2edc,	0x3067,
	0x31f1,	0x337b,	0x3505,	0x368e,	0x3817,	0x399f,	0x3b26,	0x3cad,
	0x3e34,	0x3fb9,	0x413f,	0x42c3,	0x4447,	0x45ca,	0x474d,	0x48cf,
	0x4a50,	0x4bd0,	0x4d50,	0x4ecf,	0x504d,	0x51cb,	0x5347,	0x54c3,
	0x563e,	0x57b8,	0x5932,	0x5aaa,	0x5c22,	0x5d98,	0x5f0e,	0x6083,
	0x61f7,	0x636a,	0x64dc,	0x664d,	0x67bd,	0x692d,	0x6a9b,	0x6c08,
	0x6d74,	0x6edf,	0x7049,	0x71b2,	0x7319,	0x7480,	0x75e5,	0x774a,
	0x78ad,	0x7a0f,	0x7b70,	0x7cd0,	0x7e2e,	0x7f8b,	0x80e7,	0x8242,
	0x839c,	0x84f4,	0x864b,	0x87a1,	0x88f5,	0x8a48,	0x8b9a,	0x8cea,
	0x8e39,	0x8f87,	0x90d3,	0x921e,	0x9368,	0x94b0,	0x95f6,	0x973b,
	0x987f,	0x99c1,	0x9b02,	0x9c41,	0x9d7f,	0x9ebb,	0x9ff6,	0xa12f,
	0xa267,	0xa39d,	0xa4d1,	0xa604,	0xa736,	0xa865,	0xa993,	0xaac0,
	0xabeb,	0xad14,	0xae3b,	0xaf61,	0xb085,	0xb1a7,	0xb2c8,	0xb3e7,
	0xb504,	0xb620,	0xb739,	0xb851,	0xb968,	0xba7c,	0xbb8e,	0xbc9f,
	0xbdae,	0xbebb,	0xbfc7,	0xc0d0,	0xc1d8,	0xc2dd,	0xc3e1,	0xc4e3,
	0xc5e3,	0xc6e1,	0xc7de,	0xc8d8,	0xc9d0,	0xcac7,	0xcbbb,	0xccae,
	0xcd9e,	0xce8d,	0xcf79,	0xd064,	0xd14c,	0xd233,	0xd317,	0xd3fa,
	0xd4da,	0xd5b9,	0xd695,	0xd76f,	0xd847,	0xd91e,	0xd9f2,	0xdac3,
	0xdb93,	0xdc61,	0xdd2c,	0xddf6,	0xdebd,	0xdf82,	0xe045,	0xe106,
	0xe1c5,	0xe281,	0xe33b,	0xe3f4,	0xe4a9,	0xe55d,	0xe60f,	0xe6be,
	0xe76b,	0xe816,	0xe8be,	0xe965,	0xea09,	0xeaab,	0xeb4a,	0xebe7,
	0xec82,	0xed1b,	0xedb2,	0xee46,	0xeed8,	0xef67,	0xeff5,	0xf07f,
	0xf108,	0xf18e,	0xf212,	0xf294,	0xf313,	0xf390,	0xf40b,	0xf483,
	0xf4f9,	0xf56d,	0xf5de,	0xf64d,	0xf6b9,	0xf723,	0xf78b,	0xf7f0,
	0xf853,	0xf8b4,	0xf912,	0xf96d,	0xf9c7,	0xfa1e,	0xfa72,	0xfac4,
	0xfb14,	0xfb61,	0xfbac,	0xfbf4,	0xfc3a,	0xfc7e,	0xfcbf,	0xfcfd,
	0xfd3a,	0xfd73,	0xfdab,	0xfde0,	0xfe12,	0xfe42,	0xfe70,	0xfe9b,
	0xfec3,	0xfeea,	0xff0d,	0xff2f,	0xff4d,	0xff6a,	0xff84,	0xff9b,
	0xffb0,	0xffc3,	0xffd3,	0xffe0,	0xffeb,	0xfff4,	0xfffa,	0xfffe
};

typedef union {
        uint8_t Data[2];
        uint16_t Word;
        } SinData;

 Point StoreArrayOfPoly[MAX_POLY_POINTS];


int32_t FastSin(uint16_t x ){
  SinData xData;  
  xData.Word = x;
  switch(xData.Data[1]){
    case 0: return (int32_t)SineTable[xData.Data[0]];
    case 1: return (int32_t)SineTable[255-xData.Data[0]];
    case 2: return -(int32_t)(SineTable[xData.Data[0]]);
    case 3: return -(int32_t)SineTable[255-xData.Data[0]];
  }
 //return

 return 0;
}
int32_t FastCos(uint16_t x){
  SinData xData;  
  xData.Word = x;
  switch(xData.Data[1]){
    case 3: return (int32_t)SineTable[xData.Data[0]];
    case 0: return (int32_t)SineTable[255-xData.Data[0]];
    case 1: return -(int32_t)(SineTable[xData.Data[0]]);
    case 2: return -(int32_t)SineTable[255-xData.Data[0]];
  }
 //ret
 return 0;
}

Point RotatePoint(Point Coord, Point Coord0, uint16_t angle){ //angle max is 2pi rad 
float32_t cosV;
float32_t sinV;
float32_t Xo;
float32_t Yo;
float32_t X;
float32_t Y;


 // angle*= 0.017453292f;
  cosV = ((float)(FastCos((uint16_t) angle)))*1.52587890625e-5;// arm_cos_f32(angle);
  sinV = ((float)(FastSin((uint16_t) angle)))*1.52587890625e-5;// arm_sin_f32(angle);
  
  X = (float32_t)Coord.X;
  Y = (float32_t)Coord.Y;
  Xo = (float32_t)Coord0.X;
  Yo = (float32_t)Coord0.Y;

  Coord.X = (uint16_t)(Xo + (((X - Xo)*cosV)) + (((Yo - Y)*sinV)));
  Coord.Y = (uint16_t)(Yo + (((X - Xo)*sinV)) + (((Y - Yo)*cosV)));

return Coord;
}

void RotatePoly(Point* pToPoints, uint8_t NumbOfPoints, const pPoint Origin, uint32_t angle_deg){
  int i;

  uint16_t angle = angle_deg/352;//2.8444443592513164e-3 * (float32_t)angle_deg;  
  for(i = 0; i < NumbOfPoints; i++){
   pToPoints[i] = RotatePoint(pToPoints[i], *Origin, angle);
   ///!! Magic?!
  // if(pToPoints[i].X == OldOne.X && pToPoints[i].Y == OldOne.Y)
   // break;
  }
  RotatedF = 1;
}
Point* StorePoly(const Point* pToPoints, uint8_t NumbOfPoints) //we can store our poly before rotation 
{
  uint8_t i;
  if(NumbOfPoints < MAX_POLY_POINTS){
  if(!RotatedF){  
    for(i = 0; i < NumbOfPoints; i++) StoreArrayOfPoly[i] = pToPoints[i];
  

    RotatedF = 1;
   }
  }
  return StoreArrayOfPoly;
}

void RestorePoly(Point* pToPoints, uint8_t NumbOfPoints) //and restore it after to iliminate degradation 
{
  uint8_t i;
   if(NumbOfPoints < MAX_POLY_POINTS){
     if(RotatedF){
       for(i = 0; i < NumbOfPoints; i++) pToPoints[i] = StoreArrayOfPoly[i];
       RotatedF = 0;
     }
  }
}

uint8_t * Itoa(uint8_t * StrDst, uint16_t Number){ // convert int into string

  uint8_t * pStrDst = &StrDst[0];
  uint16_t Tmp;
  uint8_t Iliminate = 1;
  
  Tmp = Number/10000;
  if(Tmp)*pStrDst++ = Tmp + 0x30, Iliminate = 0;
    Number -= Tmp*10000;
  Tmp = Number/1000;
  if(Tmp) *pStrDst++ = Tmp + 0x30, Iliminate = 0;
  else 
    if (!Iliminate) *pStrDst++ = 0x30;
  Number -= Tmp*1000;
  Tmp = Number/100;
  if(Tmp) *pStrDst++ = Tmp + 0x30, Iliminate = 0;
    else 
    if (!Iliminate) *pStrDst++ = 0x30;
  Number -= Tmp*100;
  Tmp = Number/10;
  if(Tmp) *pStrDst++ = Tmp + 0x30, Iliminate = 0;
    else 
    if (!Iliminate) *pStrDst++ = 0x30;
  Tmp = Number%10;
  *pStrDst++ = Tmp + 0x30;
  *pStrDst = '\0';
return &StrDst[0];
}

