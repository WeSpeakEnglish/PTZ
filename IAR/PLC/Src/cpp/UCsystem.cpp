#include "UCsystem.h"

namespace UCsystem
{
	using namespace RGsystem;
	
	volatile bool go;
	
	UCdevice own(1);
	
	UCdevice::UCdevice(byte group):Device(DeviceType::UniversalController, group)
	{
		for(auto &channel : input)
		{
			channel.type = InputChannelType::Inactive;
			channel.newData = false;
		}
		inputsMeasurmentReady = false;
		inputsMeasureMask = 0;
	}

	uint UCdevice::getInputsMeasureMask()
	{
		return inputsMeasureMask;
	}
	
	void UCdevice::inputChannelDisable(byte channel)
	{
		if(channel<24)
		{
			input[channel].type = InputChannelType::Inactive;
			inputsMeasureMask &= ~(1 << channel);
			if(channel < 8)
			{
				GPIOA->MODER &= MODERReSet(channel);
				GPIOA->AFR[0] &= AFreset(channel);
				if(input[channel].type == InputChannelType::DiscreteOpenDown) GPIOA->PUPDR &= PUPDRReSet(channel);
				bool workWithTimer;
				workWithTimer = input[channel].type == InputChannelType::Counter || input[channel].type == InputChannelType::Frequency;
				if(channel < 4 && workWithTimer) timer2disableChannel(channel);
			}
		}
	}
	
	void UCdevice::inputChannelEnable(byte channel, InputChannelType type)
	{
		if(channel < 24 && type != InputChannelType::Inactive)
		{
			inputChannelDisable(channel);
			if(channel < 8)
			{
				if(type == InputChannelType::Frequency || type == InputChannelType::Counter)
				{
					if(channel < 4)
					{
						GPIOA->MODER |= MODERSet(channel, ModeAf);
						AFreg(GPIOA) |= AFset(channel, 1);
						inputsMeasureMask |= 1 << channel;
						input[channel].type = type;
						timer2enableChannel(channel, (type == InputChannelType::Frequency) ? true : false);
					}
				}
				else
				{
					GPIOA->MODER &= MODERReSet(channel);
					if(type == InputChannelType::Voltage || type == InputChannelType::Current) GPIOA->MODER |= MODERSet(channel, ModeAnalog);
					else if(type == InputChannelType::DiscreteOpenDown) GPIOA->PUPDR |= PUPDRSet(channel, PupUP);
					inputsMeasureMask |= 1 << channel;
					input[channel].type = type;
				}
			}
			else if(type <= InputChannelType::Current)
			{
				inputsMeasureMask |= 1 << channel;
				input[channel].type = type;
			}
		}
	}
	
	bool getChanelIndex(ParameterID ID, byte &value)
	{
		bool temp = false;
		if(ID >= ParameterID::InputChannel_0 && ID <= ParameterID::InputChannel_23)
		{
			value = (byte)ID - (byte)ParameterID::InputChannel_0;
			temp = true;
		}
		return temp;
	}
	
	ParameterID getParameterID(byte channel)
	{
		return (ParameterID)(channel + (ushort)ParameterID::InputChannel_0);
	}
	
	MessageHeader* UCdevice::messagePrepare()
	{
		ParameterHeader *paramHeader;
		MessageHeader *mesHeader = (MessageHeader*)transmitter.buf;
		if(inputsAsk.Mask > 0)// checking for parameter to send
		{
			transmitter.bufCounter = 8;
			for(byte i = 0; i < 24; ++i)
			{
				if((inputsAsk.Mask & (1 << i)) > 0)
				{
					InputChannel *channel = &input[i];
					paramHeader = (ParameterHeader*)&transmitter.buf[transmitter.bufCounter];
					paramHeader->ID = getParameterID(i);
					if(channel->getType() <= InputChannelType::Current)// for 3 byte parameter
					{
						paramHeader->type = ParameterType::Single;
						((Parameter8*)paramHeader)->data = channel->value.Single;
						transmitter.bufCounter += 3;
					}
					else// for 4 byte parameter
					{
						paramHeader->type = ParameterType::Double;
						((Parameter16*)paramHeader)->data = channel->value.Double;
						transmitter.bufCounter += 4;
					}
				}
			}
			inputsAsk.Mask = 0;
			if(transmitter.bufCounter > 8 && (transmitter.bufCounter <= sizeof(transmitter.buf)) )
			{
				transmitter.CANID = CANdeviceID::UniversalController;
				transmitter.group = mGroup;
				transmitter.owner = BufferOwner::Handler;
				mesHeader->counter = 0;
				mesHeader->group = mGroup;
				mesHeader->messageLen = transmitter.bufCounter - 8;
				mesHeader->messageCRC = CRCcalculateBuf(&transmitter.buf[8], mesHeader->messageLen);
			}
		}
		else mesHeader->messageLen = 0;
		return mesHeader;
	}
	
	void UCdevice::messageReceived()
	{
		ParameterHeader *header;
		InputChannel *channel;
		byte channelIndex;
		receiver.bufCounter = 0;
		while(receiver.bufCounter < receiver.messageLength)
		{
			header = (ParameterHeader*)&receiver.buf[receiver.bufCounter];
			switch(header->type)
			{
				case (byte)ParameterType::Single:
					if(getChanelIndex(header->ID, channelIndex))// if input channel
					{
						channel = &input[channelIndex];
						byte val = receiver.buf[receiver.bufCounter+sizeof(ParameterHeader)];
						if(channel->value.Voltage != val)channel->hasChanged = true;
						channel->value.Single= val;
						channel->newData = true;
					}
					receiver.bufCounter += sizeof(ParameterType::Single) + sizeof(ParameterHeader);
				break;
				case (byte)ParameterType::Double:
					if(getChanelIndex(header->ID, channelIndex))// if input channel
					{
						channel = &input[channelIndex];
						ushort val = *((ushort*)&receiver.buf[receiver.bufCounter+sizeof(ParameterHeader)]);
						if(channel->value.Frequency != val)channel->hasChanged = true;
						channel->value.Double = val;
						channel->newData = true;
					}
					receiver.bufCounter += sizeof(ParameterType::Double) + sizeof(ParameterHeader);
				break;
			}
		}
	}
	
	void UCdevice::deviceManager()
	{
		if(receiver.messageNew)
		{
			if(receiver.owner == BufferOwner::Free)
			{
				receiver.owner = BufferOwner::Handler;
				receiver.messageNew = false;
				if(CRCcalculateBuf(receiver.buf, receiver.messageLength) == receiver.messageCRC)messageReceived();
				receiver.owner = BufferOwner::Free;
			}
		}
		bool shouldSendMessage = (inputsAsk.Mask > 0) && CAN2ready() && (transmitter.owner == BufferOwner::Free);
		if(shouldSendMessage)
		{
			MessageHeader *header = messagePrepare();
			if(header->messageLen > 0)
			{
				CAN2TransmitPack((ushort)CANdeviceID::UniversalController, (byte*)header, 8);
				transmitter.bufCounter -= 8;
			}
		}
	}
	
	void UCdevice::clock100Hz()
	{
		
	}
	
	void UCdevice::clock10Hz()
	{
		
	}
	
	void UCdevice::clock1Hz()
	{
		//inputsAskMask = 1;
	}

	void deviceInit()
	{
		systemLedsConfig();
		CANinit();
		ADC1init();
		ADC2init();
		timer2init();
		I2C1init();
	}

	void inputsCheck()// 100Hz
	{
		static byte ADClag;
		if(own.getInputsMeasureMask() > 0)
		{
			if(ADCbothReady())
			{
				ADClag = 0;
				
				bool presentAnalog = false;// Check for active analog inputs, that should me measured
				bool discrete;
				
				bool ADC2inc = false;
				
				ushort counter;
				static byte counterMeasureCnt[4];
				static byte counterMeasureCntMax[4] = {99,99,99,99};
				static byte multiplier[4] = {8,8,8,8};
				
				if((own.getInputsMeasureMask() & 0xFF) > 0)// For analog channels 0-7
				{
					for(byte i = 0; i < 8; ++i)
					{
						InputChannelType type = own.input[i].getType();
						switch(type)
						{
							case InputChannelType::DiscreteDirect:
							case InputChannelType::DiscreteOpenDown:
							case InputChannelType::DiscreteOpenUp:
								discrete = (GPIOA->IDR & (1 << i)) > 0;
								if(type == InputChannelType::DiscreteOpenDown) discrete = !discrete;
								if(own.input[i].value.Discrete != discrete) own.input[i].hasChanged = true;
								own.input[i].value.Discrete = discrete;
								own.input[i].newData = true;
							break;
							
							case InputChannelType::Voltage:
							case InputChannelType::Current:
								presentAnalog = true;
							break;
							
							case InputChannelType::Counter:
							case InputChannelType::Frequency:
								if(++counterMeasureCnt[i] > counterMeasureCntMax[i])
								{
									counterMeasureCnt[i] = 0;
									if(type == InputChannelType::Counter) counter = timer2getCounter(i);
									else
									{
										counter = timer2getFrequency(i);
										counterMeasureCntMax[i] = 19;
									}
									if(counter < 100)
									{
										if(multiplier[i] != 1 && counter < 95)// lower threshold
										{
											timer2setMultiplier(i, 1);
											multiplier[i] = 1;
										}
									}
									else if(counter < 200)
									{
										if(multiplier[i] != 2 && (counter > 105 && counter < 194))// lower and upper threshold
										{
											timer2setMultiplier(i, 2);
											multiplier[i] = 2;
										}
									}
									else if(counter < 400)
									{
										if(multiplier[i] != 4 && (counter > 210 && counter < 388))// lower and upper threshold
										{
											timer2setMultiplier(i, 4);
											multiplier[i] = 4;
										}
									}
									else if(counter < 800)
									{
										if(multiplier[i] != 8 && (counter > 420 && counter < 776))// lower and upper threshold
										{
											timer2setMultiplier(i, 8);
											multiplier[i] = 8;
										}
									}
									if(type == InputChannelType::Counter)
									{
										if(own.input[i].value.Counter != counter) own.input[i].hasChanged = true;
										own.input[i].value.Counter = counter;	
									}
									else
									{
										if(own.input[i].value.Frequency != counter) own.input[i].hasChanged = true;
										own.input[i].value.Frequency = counter;	
									}
									own.input[i].newData = true;
								}
							break;
						}
					}
					if(presentAnalog)ADC1start();// If we have one or more active analog channels
				}
				if((own.getInputsMeasureMask() & ~(uint)0xFF) > 0)
				{
					ADC2inc = true;
					ADC2start();// For analog channels 8-23
				}
				if(!ADC2inc && !presentAnalog) own.inputsMeasurmentReady = true;
			}
			else if(++ADClag > 99) ADClagReset();
		}
	}
	
	void clock100Hz()
	{
		//inputsCheck();// Checking all UC inputs if they enabled
		//sysAHRtick();
	}
	
	void clock10Hz()
	{
		sysLedGreenInvert();
		own.clock10Hz();
		//inputsCheck();// Checking all UC inputs if they enabled
	}
	
	void clock1Hz()
	{
		own.clock1Hz();
		inputsCheck();// Checking all UC inputs if they enabled
		go = true;
//		static byte loo;
//		CAN2TransmitByte(0x7F7,loo++);
	}
	
	void deviceTick()// From system timer 1000Hz
	{
		static byte delay10ms;
		static byte delay100ms;
		static byte delay1sec;
		
		if(++delay10ms > 9)
		{
			delay10ms = 0;
			clock100Hz();// 0.01 sec, 100Hz
			if(++delay100ms > 9)
			{
				delay100ms = 0;
				clock10Hz();// 0.1 sec, 10Hz
				if(++delay1sec > 9)
				{
					delay1sec = 0;
					clock1Hz();// 1 sec, 1Hz
				}
			}
		}
	}
}
