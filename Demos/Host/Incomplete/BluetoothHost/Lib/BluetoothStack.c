/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this 
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in 
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting 
  documentation, and that the name of the author not be used in 
  advertising or publicity pertaining to distribution of the 
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "BluetoothStack.h"

/** Bluetooth device connection information structure. Once connected to a remote device, this structure tracks the
 *  connection state of the individual L2CAP channels.
 */
Bluetooth_Connection_t Bluetooth_Connection = {IsConnected: false};

/** Bluetooth configuration structure. This structure configures the bluetooth stack's user alterable settings. */
Bluetooth_Device_t Bluetooth_DeviceConfiguration =
	{
		Class:   (DEVICE_CLASS_SERVICE_CAPTURING | DEVICE_CLASS_MAJOR_COMPUTER | DEVICE_CLASS_MINOR_COMPUTER_PALM),
		PINCode: "0000",
		Name:    "LUFA Bluetooth Demo"
	};

void Bluetooth_Stack_Init(void)
{
	Bluetooth_HCIProcessingState = Bluetooth_Init;
}

void Bluetooth_Stack_USBTask(void)
{
	Bluetooth_ProcessHCICommands();
	Bluetooth_ProcessACLPackets();
}

bool CALLBACK_Bluetooth_ConnectionRequest(uint8_t* RemoteAddress)
{
	/* Always accept connections from remote devices */
	return true;
}

Bluetooth_Channel_t* Bluetooth_GetChannelData(uint16_t ChannelNumber, bool SearchBySource)
{
	Bluetooth_Channel_t* CurrentChannelStructure;

	for (uint8_t i = 0; i < BLUETOOTH_MAX_OPEN_CHANNELS; i++)
	{
		CurrentChannelStructure = &Bluetooth_Connection.Channels[i];
	
		uint16_t CurrentChannelNumber = ((SearchBySource) ? CurrentChannelStructure->RemoteNumber : CurrentChannelStructure->LocalNumber);
	
		if (CurrentChannelNumber == ChannelNumber)
		  return CurrentChannelStructure;
	}

	return NULL;
}

Bluetooth_Channel_t* Bluetooth_InitChannelData(uint16_t RemoteChannelNumber, uint16_t PSM)
{
	Bluetooth_Channel_t* CurrentChannelStructure;

	for (uint8_t i = 0; i < BLUETOOTH_MAX_OPEN_CHANNELS; i++)
	{
		CurrentChannelStructure = &Bluetooth_Connection.Channels[i];
	
		if (CurrentChannelStructure->State == Channel_Closed)
		{
			CurrentChannelStructure->RemoteNumber = RemoteChannelNumber;
			CurrentChannelStructure->LocalNumber  = (BLUETOOTH_CHANNELNUMBER_BASEOFFSET + i);
			CurrentChannelStructure->PSM          = PSM;
			CurrentChannelStructure->State        = Channel_Config;
			
			return CurrentChannelStructure;
		}		
	}

	return NULL;
}
