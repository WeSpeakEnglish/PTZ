#include "RGsystem.h"

#ifdef UC_DEVICE
#include "../UCsystem/UCsystem.h"
#endif
#ifdef ANS_DEVICE
#include "../ANSsystem/ANS-3.h"
#endif

namespace RGsystem
{	
	#ifdef UC_DEVICE
	//using namespace UCsystem;
	#endif
	#ifdef ANS_DEVICE
	using namespace ANSsystem;
	#endif
	
	activeDevice projectDevice[DEVICE_MAX_NUMBER];
	byte deviceCount=0;
	CANbufT transmitter;
	
	Device::Device(DeviceType type, byte group)
	{
		if(group<32)mGroup=group;
		else mGroup=0;
		
		if(deviceCount<DEVICE_MAX_NUMBER)
		{
			projectDevice[deviceCount].device=this;
			projectDevice[deviceCount].type=type;
			deviceCount++;
		}
	}
	
	byte Device::getGroup()
	{
		return mGroup;
	}
	
	void Device::CANpackReceived(byte *CANpack)
	{
		static ushort packetCounter = 0;
		if(receiver.owner == BufferOwner::Free || receiver.owner == BufferOwner::CAN)
		{
			CANpackBody *body = (CANpackBody*)CANpack;
			if(body->group == mGroup)
			{
				MessageHeader *newMessage = (MessageHeader*)CANpack;
				receiver.overflow = false;
				if(newMessage->isStart())// if this is new message
				{
					receiver.bufCounter = 0;
					if(newMessage->messageLen < 2)
					{
						packetCounter = 0;
						return;
					}
					else if(newMessage->messageLen > sizeof(receiver.buf))
					{
						packetCounter = 0;
						receiver.overflow = true;
						return;
					}
					receiver.owner = BufferOwner::CAN;
					packetCounter = 1;
					receiver.messageLength = newMessage->messageLen;
					receiver.messageCRC = newMessage->messageCRC;
				}
				else if(body->counter == packetCounter)
				{
					bool isLastPack = false;
					ushort currentNumber = receiver.messageLength - (packetCounter - 1) * 6;// Number of useful data bytes left
					if(currentNumber > 6) currentNumber = 6;// Current number of useful data bytes in CANpack
					else isLastPack = true;
					for(byte i = 0; i < currentNumber; i++, receiver.bufCounter++) receiver.buf[receiver.bufCounter] = body->data[i];// Filling the buffer
					if(isLastPack)
					{
						receiver.messageNew = true;
						receiver.owner = BufferOwner::Free;
						packetCounter = 0;
						receiver.bufCounter = 0;
					}
					if(!receiver.messageNew) packetCounter++;
				}
			}
		}
	}
	
	void sendMessageBody()
	{
		static ushort step = 0;
		if(transmitter.bufCounter > 0)
		{
			CANpackBody body;
			byte usefulCnt = (transmitter.bufCounter > 6) ? 6 : transmitter.bufCounter;
			body.counter = step + 1;
			body.group = transmitter.group;
			for(byte i = 0; i < usefulCnt; ++i) body.data[i] = transmitter.buf[8 + step * 6 + i];//transmitter.buf
			transmitter.bufCounter -= usefulCnt;
			#ifdef UC_DEVICE
			UCsystem::CAN2TransmitPack((ushort)transmitter.CANID, (byte*)&body, usefulCnt + 2);
			#endif
			step++;
		}
		else
		{
			step = 0;
			transmitter.owner = BufferOwner::Free;
		}
	}
	
	void systemTick()
	{
		#ifdef UC_DEVICE
		UCsystem::deviceTick();
		#endif
		//ANSsystem::deviceTick();
	}
	
	void systemManager()
	{
		for(byte i=0;i<deviceCount;++i)
		{
			switch(projectDevice[i].type)
			{
				#ifdef UC_DEVICE
				case DeviceType::UniversalController:
					((UCsystem::UCdevice*)projectDevice[i].device)->deviceManager();
				break;
				#endif
//				case DeviceType::AngleSensor:
//					((ANSdevice*)projectDevice[i].device)->deviceManager();
//				break;
			}
		}
	}
	
	bool isRGdevice(ushort ID)
	{
		for(auto i = (ushort)CANdeviceID::BroadCast; i <= (ushort)CANdeviceID::Debugger; ++i)
		{
			if(ID == i) return true;
		}
		return false;
	}
	
	void initialize()
	{
		sysTimerConfig();
		//WDinit();
		CRCinit();
		#ifdef UC_DEVICE
		UCsystem::deviceInit();
		#endif
		#ifdef ANS_DEVICE
		ANSsystem::deviceInit();
		#endif
	}
}
