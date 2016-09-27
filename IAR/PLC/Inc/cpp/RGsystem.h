#pragma once

typedef unsigned char uint8;
typedef unsigned char byte;
typedef signed char int8;

typedef unsigned short int uint16;
typedef unsigned short ushort;
typedef signed short int int16;

typedef unsigned int uint;
typedef unsigned int uint32;

typedef unsigned long long uint64;
typedef signed long long int64;

typedef struct
{
	byte year;		// год(00-99)
	byte month;		// мес¤ц(01-12)
	byte day;		// день мес¤ца(01-31)
	byte hour;		// часы(00-23)
	byte minute;	// минуты(00-59)
	byte second;	// секунды(00-59)
	uint epoch;		// секунды с 1970
}DateTimeT;


#include "SysTimer.h"
#include "CRC.h"
#include "Wdog.h"


#define UC_DEVICE

namespace RGsystem
{
#define DEVICE_MAX_NUMBER 10
	
	enum class DeviceType : byte
	{
		UniversalController,
		OnBoardComputer,
		AngleSensor
	};
	
	enum class CANdeviceID : ushort
	{
		BroadCast=0x7F0,
		OnBoardComputer,
		UniversalController,
		AngleSensor=0x7F5,
		Debugger=0x7FF,
		Mask=0x7F0
	};
	enum class ParameterID : ushort
	{
		CoreID,
		InputChannel_0=0x30,
		InputChannel_1,
		InputChannel_2,
		InputChannel_3,
		InputChannel_4,
		InputChannel_5,
		InputChannel_6,
		InputChannel_7,
		InputChannel_8,
		InputChannel_9,
		InputChannel_10,
		InputChannel_11,
		InputChannel_12,
		InputChannel_13,
		InputChannel_14,
		InputChannel_15,
		InputChannel_16,
		InputChannel_17,
		InputChannel_18,
		InputChannel_19,
		InputChannel_20,
		InputChannel_21,
		InputChannel_22,
		InputChannel_23,
		InputChannel_24,
		InputChannel_25,
		InputChannel_26,
		InputChannel_27,
		InputChannel_28,
		InputChannel_29,
		InputChannel_30,
		InputChannel_31,
		InputChannel_32,
		InputChannel_33,
		InputChannel_34,
		InputChannel_35,
		InputChannel_36,
		InputChannel_37,
		InputChannel_38,
		InputChannel_39,
		InputChannel_40,
		InputChannel_41,
		InputChannel_42,
		InputChannel_43,
		InputChannel_44,
		InputChannel_45,
		InputChannel_46,
		InputChannel_47,
	};
	
	enum class ParameterType : byte
	{
		Get,
		Set,
		Single,
		Double,
		Quad,
		Flow,
		reserved,
		Error
	};
	
	struct ParameterHeader
	{
		ParameterType	type	: 3;
		ParameterID		ID		: 13;
	};
 
	struct Parameter8
	{
		ParameterType	type	: 3;
		ParameterID		ID		: 13;
		byte			data;
	};
	
	struct Parameter16
	{
		ParameterType	type	: 3;
		ParameterID		ID		: 13;
		ushort			data;
	};
	
	struct MessageHeader
	{
		ushort	counter	: 11;
		byte	group	: 5;
		ushort	messageLen;
		uint	messageCRC;
		bool	isStart(){return counter == 0;};
	};
	
	struct CANpackBody
	{
		ushort	counter	: 11;
		byte	group	: 5;
		byte	data[6];
	};
	
	enum class BufferOwner
	{
		Free,
		CAN,
		Handler
	};
	
	struct CANbufT
	{
		BufferOwner	owner;
		CANdeviceID	CANID;
		byte		group;
		uint		messageCRC;
		ushort		messageLength;
		bool		messageNew;
		bool		overflow;
		byte		buf[200];
		ushort		bufCounter;
		CANbufT()
		{
			bufCounter = 0;
			overflow = false;
			messageNew = false;
			owner = BufferOwner::Free;
		}
	};
	
	class Device
	{
	protected:
		CANbufT receiver;
		byte	mGroup;
	public:
		Device(DeviceType type, byte group=0);
		byte getGroup();
		void CANpackReceived(byte *CANpack);
	};
	
	struct activeDevice
	{
		Device *device;
		DeviceType type;
	};
		
	extern activeDevice projectDevice[DEVICE_MAX_NUMBER];//deviceArray
	extern byte deviceCount;
	extern CANbufT transmitter;
	
	void sendMessageBody();
	void systemTick();
	void systemManager();
	bool isRGdevice(ushort ID);
	void initialize();
}
