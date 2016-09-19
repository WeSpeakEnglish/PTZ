#ifndef _CALCULATIONS_H
#define _CALCULATIONS_H
#include <stdint.h>
#include <arm_math.h>
#include "lcd.h"
#include "gui.h"

#define MAX_POLY_POINTS 24
extern Point StoreArrayOfPoly[MAX_POLY_POINTS];

int32_t FastSin(uint16_t); // 2^10 = 360 degrees 
int32_t FastCos(uint16_t); // 2^10 = 360 degrees 


Point RotatePoint(Point Coord, Point Coord0, uint16_t angle); //angle in degrees
//void RotatePoly(Point* pToPoints, uint8_t NumbOfPoints, const pPoint Origin, uint32_t angle_deg);
//Point * StorePoly(const Point* pToPoints, uint8_t NumbOfPoints); //we can store our poly before rotation 
//void RestorePoly(Point* pToPoints, uint8_t NumbOfPoints); //and restore it after to iliminate degradation 
uint8_t * Itoa_R(uint8_t * StrDst, uint8_t SizeOfStr, int16_t Number);
inline void RotatePoly(Point* pToPoints, uint8_t NumbOfPoints, const pPoint Origin, uint32_t angle_deg){
  int i;

  uint16_t angle = angle_deg/352;//2.8444443592513164e-3 * (float32_t)angle_deg;
  
  for(i = 0; i < NumbOfPoints; i++){
    pToPoints[i] =  RotatePoint(pToPoints[i], *Origin, angle);
  }
  RotatedF = 1;
}

inline Point* StorePoly(const Point* pToPoints, uint8_t NumbOfPoints) //we can store our poly before rotation 
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

inline void RestorePoly(Point* pToPoints, uint8_t NumbOfPoints) //and restore it after to iliminate degradation 
{
  uint8_t i;
  if(NumbOfPoints < MAX_POLY_POINTS){
    if(RotatedF){
      for(i = 0; i < NumbOfPoints; i++) pToPoints[i] = StoreArrayOfPoly[i];
      RotatedF = 0;
    }
  }
}

uint8_t * Ftoa2(uint8_t * StrDst, float NumberF);
uint8_t * Itoa(uint8_t * StrDst, int16_t Number);  // convert int16 into string 
uint8_t * Utoa(uint8_t * StrDst, uint16_t Number); // convert unsigned int 16 into string 
uint8_t * Ftoa_R(uint8_t * StrDst, uint8_t SizeOfStr, float NumberF);
#endif
