#ifndef PROTOCOL_INTERFACE_H
#define PROTOCOL_INTERFACE_H
#include "PTZinterface.h"
#include "protocol_interface.h"
#include "can_exchange.h"


enum {
 Tachometer =                           0x30,
 Spedometer =                           0x31,
 CoolantTemperature =                   0x32,
 FuelLevel =                            0x33,
 GearboxOilPress =                      0x34,
 GearboxOilTemp =                       0x35,
 PneumaticAirPress =                    0x36,
 EngineOilPress =                       0x37,
 Neutral =                              0x38,
 ParkingBrake =                         0x39,
 //highCoolantTemperature =               0x3A,
 RearAxle =                             0x3A,
 CloggedAirFilter =                     0x3B,
 CoolantLow =                           0x3C,
 InfestationCPRFilter =                 0x3D, //13
 PTO =                                  0x3E, //14
 //EmergencyEngineOilPress =              0x3E,
 CriticalEngineOilTemp =                0x3F,
 InfestationEngineFilter =              0x40,
 InfestationSteeringFilter =            0x41,
 InfestationAttachmentFilter =          0x42,
 HydraulicOilLow =                      0x43, //19 Oil GB
 FarLight =                             0x44,
// HydraulicHighTemp =                    0x44,
 ErrAirPressBrake_1 =                   0x45,
 ErrAirPressBrake_2 =                   0x46,
 BatterySgn =                           0x47,
 DateTime =                             800
} ID_Param;

#endif



#define FUEL_MULTIPLIER         1.0f
#define AIRPRESS_MULTIPLIER     1.0f

void CAN_GetValues(){
  uint16_t i;
  if(!pasreCAN()){
  for(i = 0; i < SIZE_CAN_DATA; i++){
    switch(CAN_Data_dID[i]){
    case Tachometer:
      break;
    case Spedometer:
      PTZ.Speed = (float)CAN_data.dData[i] * 2.353f ;
      BIG_Arrow((uint16_t)PTZ.Speed);
      break;    
    case FuelLevel:
        PTZ.VolumeFuel = CAN_data.dData[i] * 0.392;//FUEL_MULTIPLIER;
        FUEL_Arrow((uint16_t)PTZ.VolumeFuel);
      break;  
    case PneumaticAirPress:
       PTZ.PressAir = (float)CAN_data.dData[i] * AIRPRESS_MULTIPLIER;
       PTZ.PressAir = (float)CAN_data.dData[i] * 0.0392;
      break;
    case FarLight:
      if(CAN_data.dData[i])PTZ.Signals.FarLight = 1; // swich on indicator
      else PTZ.Signals.FarLight = 0;
      break;
    case HydraulicOilLow:
      if(CAN_data.dData[i])PTZ.Errors.HydrotankLevel = 1;
      else PTZ.Errors.HydrotankLevel = 0; 
      break;
    case CloggedAirFilter:
      if(CAN_data.dData[i])PTZ.Errors.ImpurityAirFilter = 1;
      else PTZ.Errors.ImpurityAirFilter = 0; 
      break;
    case PTO:
      if(CAN_data.dData[i])PTZ.Signals.PTO = 1;
      else PTZ.Signals.PTO = 0; 
      break;
    case RearAxle:
      if(CAN_data.dData[i])PTZ.Signals.RearAxle = 1;
      else PTZ.Signals.RearAxle = 0; 
      break;      
    case ParkingBrake:
      if(CAN_data.dData[i])PTZ.Signals.ParkingBrake = 1;
      else PTZ.Signals.ParkingBrake = 0; 
      break; 
    case CriticalEngineOilTemp:
      if(CAN_data.dData[i])PTZ.Errors.T_CoolingLiquid = 1;
      else PTZ.Errors.T_CoolingLiquid = 0; 
      break; 
    case InfestationCPRFilter:
       if(CAN_data.dData[i])PTZ.Errors.ImpurityCoolingLiquid = 1;
       else PTZ.Errors.ImpurityCoolingLiquid = 0;
      break;
    }
  }
  }
}