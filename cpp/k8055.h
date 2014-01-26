/* k8055.h - provides class definitions for interfacing a Velleman K8055
 Copyright (C) 2005 2006 Richard Plumridge
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  US */

// K8055 Header file

#ifndef _K8055_H
#define _K8055_H

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/usbspec.h>

// Errors

#define K8055_OK			0x0000
#define K8055_GENERAL_ERR	(0x0001 << 16)
#define K8055_NO_BUF		(0x0002 << 16)
#define K8055_BAD_ADDRESS	(0x0004 << 16)
#define K8055_NOT_FOUND		(0x0008 << 16)
#define K8055_OPEN_ERR		(0x0010 << 16)

// Masks to extract data and error codes from returned values

#define K8055_DATA_MASK		0xFFFF
#define K8055_ERROR_MASK	(0xFFFF << 16)

// Class definition

class k8055
{
	
	// Public methods
	
public:
    
	// Class management
	k8055(); // Default constructor
	virtual ~k8055(); // destructor
    
	// Open and Close functions
	UInt32 OpenDevice(int address);
	void CloseDevice();
	
	// Analog channel functions
	UInt32 ReadAnalogChannel (UInt8 channel);
	UInt32 ReadAllAnalog(UInt8 *channel1 , UInt8 *channel2);
	UInt32 OutputAnalogChannel (UInt8 channel, UInt8 data);
	UInt32 OutputAllAnalog(UInt8 channel1, UInt8 channel2);
	UInt32 ClearAnalogChannel (UInt8 channel);
	UInt32 ClearAllAnalog();
	UInt32 SetAnalogChannel(UInt8 channel);
	UInt32 SetAllAnalog();
	
	// Digital channel output functions
	UInt32 WriteAllDigital(UInt8 data);
	UInt32 ClearDigitalChannel(UInt8 channel);
	UInt32 ClearAllDigital();
	UInt32 SetDigitalChannel(UInt8 channel);
	UInt32 SetAllDigital();
	
	// Digital channel output functions
	UInt32 ReadDigitalChannel(UInt8 channel);
	UInt32 ReadAllDigital();
	
	// Counter functions
	UInt32 ResetCounter(UInt8 counter);
	UInt32 ReadCounter(UInt8 counter);
	UInt32 SetCounterDebounceTime(UInt8 counter, UInt16 DebounceTime);
    
    /* Internal Stuff */
	
//protected:
	
	// Internal variables
	
	IOUSBDeviceInterface **USBDeviceInterface; // For the USB Device Interface
	IOUSBInterfaceInterface **USBInterfaceInterface; // for the USB interface
	UInt8 numEndpoints; // Number of USB endpoints
	UInt8 outPipeRef;
	UInt8 inPipeRef;
	UInt8 *buf; // Buffer
	
	UInt8 digitalOut; // digital output
	UInt8 analogOut1; // analog output
	UInt8 analogOut2; // analog output
	
	UInt8 digitalIn; // Digital input
	UInt8 analogIn1; // Analog 1 input
	UInt8 analogIn2; // Analog 2 input
	UInt16 timer1; // Timer 1
	UInt16 timer2; // Timer 2
	
	UnsignedWide lastread; //Last read time
    
	bool USBDeviceOpen;
	bool USBInterfaceOpen;
	
	// Internal methods
    
	void initialise(); // initialise
	void cleanup(); // cleanup
	void matchUSBDevice (SInt32 USBVendorId, SInt32 USBProductId, io_object_t *usbDevice);
	UInt32 createPlugIn (io_object_t *devRef, CFUUIDRef typeId, CFUUIDBytes intType, LPVOID *interface);
	UInt32 createUSBDeviceInterface (io_object_t *devRef, IOUSBDeviceInterface ***interface);
	UInt32 createUSBInterfaceInterface (io_object_t *devRef, IOUSBInterfaceInterface ***interface);
	UInt32 configureUSBDevice();
	void resetDevice();
	UInt32 outputChannels();
	UInt32 readDevice();
};


#endif

// end k8055.h
