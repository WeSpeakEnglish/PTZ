#pragma once

#include "RGsystem.h"
#include "CAN.h"
#include "ADC.h"
#include "Timer2.h"
#include "Timer3.h"
#include "Timer4.h"
#include "I2C1.h"

namespace UCsystem
{
	extern volatile bool go;
	
	enum class InputChannelType : byte
	{
		Inactive,
		DiscreteDirect,
		DiscreteOpenUp,
		DiscreteOpenDown,
		Voltage,
		Current,
		Counter,
		Frequency
	};

	union InputChannelData
	{
		friend class UCdevice;
	protected:
		byte Single;
		ushort Double;
	public:
		bool Discrete;
		byte Voltage;
		byte Current;
		ushort Counter;
		ushort Frequency;
	};

	struct InputChannel
	{
		friend class UCdevice;
		InputChannelData value;
		bool newData;
		bool hasChanged;
		InputChannelType getType(){return type;};
	protected:
		InputChannelType type;
	};
	
	class UCdevice : public RGsystem::Device
	{
		friend void RGsystem::systemManager();
		
	private:
		uint	inputsMeasureMask;// for system measurment
	
		RGsystem::MessageHeader* messagePrepare();
		void messageReceived();
		void deviceManager();
	
	public:
		InputChannel input[24];
		bool inputsMeasurmentReady;
	
		struct Ask
		{
		private:
			friend class UCdevice;
			//uint Periodic;
		public:
			uint Periodic;
			uint Mask;
			Ask()
			{
				Mask = 0;
				Periodic = 0;
			}
		}inputsAsk;

		UCdevice(byte group=0);
		uint getInputsMeasureMask();
		
		void inputChannelEnable(byte channel, InputChannelType type);
		void inputChannelDisable(byte channel);

		void clock100Hz();
		void clock10Hz();
		void clock1Hz();
	};

	extern UCdevice own;

	void deviceInit();
	void inputsCheck();
	void deviceTick();
}
