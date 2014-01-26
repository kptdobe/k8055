/* k8055.cpp - provides library functions for interfacing a Velleman K8055
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

// Core methods for Velleman K8055

#include <k8055.h>

// Definitions for K8055

#define READ_INTERVAL 200 // in ms - 0.2 second read interval when requested - for high speed applications might need to change this

#define kK8055VendorId	0x10CF
#define kK8055Card0Id	0x5500
#define kK8055Card1Id	0x5501
#define kK8055Card2Id	0x5502
#define kK8055Card3Id	0x5503

#define BUFSIZE 8

#define K8055_SWITCH1	16
#define K8055_SWITCH2	32
#define K8055_SWITCH3	1
#define K8055_SWITCH4	64
#define K8055_SWITCH5	128

#define K8055_SET		255
#define K8055_CLEAR		0

// Code starts here

// Initialise default data structures

void k8055::initialise()
{
	
	USBDeviceInterface=0; // For the USB Device Interface
	USBInterfaceInterface=0; // for the USB interface
	numEndpoints=0; // Number of USB endpoints
	outPipeRef=0; // Out USB pipe Ref
	inPipeRef=0; // In USB pipe Ref
	
	USBDeviceOpen=FALSE; // Device is Open
	USBInterfaceOpen=FALSE; // Interface is Open
	
	digitalIn=0; // Digital input
	analogIn1=0; // Analog 1 input
	analogIn2=0; // Analog 2 input
	timer1=0; // Timer 1
	timer2=0; // Timer 2
	
	lastread.hi=0;	// Clear for the last read time
	lastread.lo=0;
}

void k8055::cleanup()
{
    
	if (USBInterfaceOpen)
    {
		(*USBInterfaceInterface)->USBInterfaceClose(USBInterfaceInterface); // Close it
		USBInterfaceOpen=FALSE;
    }
    
	if (USBDeviceOpen)
    {
		(*USBDeviceInterface)->USBDeviceClose(USBDeviceInterface); // Close it
		USBDeviceInterface=FALSE;
    }
    
	if (USBDeviceInterface)
    {
		(*USBDeviceInterface)->Release(USBDeviceInterface); // Tidy up
		USBDeviceInterface=0;
    }
    
	if (USBInterfaceInterface)
    {
		(*USBInterfaceInterface)->Release(USBInterfaceInterface); // Tidy up
		USBInterfaceInterface=0;
    }
    
}

// Constructor

k8055::k8055()
{
	initialise(); // Initialise internal variables
	
	buf=0;
	buf=new UInt8 [BUFSIZE]; // Initialise the memory for the buffer
}

// Destructor

k8055::~k8055()
{
	cleanup(); // Clean up before destruction
    
	delete [] buf;
}

// This function sets up a matching dictionary and matches the requested USB device

void k8055::matchUSBDevice (SInt32 USBVendorId, SInt32 USBProductId, io_object_t *usbDevice)

{
	CFMutableDictionaryRef  matchingDict = NULL; // For setting up the matching dictionary
    
	(*usbDevice)=0;
	
	matchingDict = IOServiceMatching(kIOUSBDeviceClassName); // Setup the matching dictionary
	
	if (matchingDict)
	{
		CFDictionaryAddValue(matchingDict, CFSTR(kUSBVendorID), CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &USBVendorId)); // Add value for the VendorId
		CFDictionaryAddValue(matchingDict, CFSTR(kUSBProductID), CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &USBProductId)); // Add value for the ProductID
        
		(*usbDevice)=IOServiceGetMatchingService(kIOMasterPortDefault, matchingDict ); // Match
	}
    
}

// Function to create the intermediate plugins and Query the interface
// Returns K8055_PLUGIN_OK if successful

UInt32 k8055::createPlugIn (io_object_t *devRef, CFUUIDRef typeId, CFUUIDBytes intType, LPVOID *interface)

{
    
	IOReturn err;
	IOCFPlugInInterface         **plugInInterface = NULL;
	SInt32                      score;
    
    // Get the IUnknown interface for the object of type typeId
    
	err = IOCreatePlugInInterfaceForService(*devRef,typeId,kIOCFPlugInInterfaceID,&plugInInterface, &score);
    
	if ((err != kIOReturnSuccess) || !plugInInterface)
    {
        return K8055_GENERAL_ERR; // If the IUnknown interface is not implemented or an object of that typeid didn't exist
    }
	
	// Lookup the required client interface as defined in intType
	
	err = (*plugInInterface)->QueryInterface(plugInInterface,intType,interface);
	
	// Don't need the intermediate interface once we have the interface of intType
	
	(*plugInInterface)->Release(plugInInterface);
	
	// Should now have an interface to intType
	
	if (err || !interface)
    {
        return K8055_GENERAL_ERR;
    }
	
	return K8055_OK;
    
}

// Create a USB device interface

UInt32 k8055::createUSBDeviceInterface (io_object_t *devRef, IOUSBDeviceInterface ***interface)
{
    
	return createPlugIn(devRef,kIOUSBDeviceUserClientTypeID,CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID),(LPVOID *)interface);
    
}

// Create a USB interface interface

UInt32 k8055::createUSBInterfaceInterface (io_object_t *devRef, IOUSBInterfaceInterface ***interface)
{
    
	return createPlugIn(devRef,kIOUSBInterfaceUserClientTypeID,CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID),(LPVOID *)interface);
    
}

// Configure USB devices

UInt32 k8055::configureUSBDevice()
{
    
    // Currently does nothing here
    
    return K8055_OK;
}

// Open the device

UInt32 k8055::OpenDevice(int address)
{
	io_object_t	usbDevice;	// used by USB device matching
	io_object_t usbInterface; // used for the interface
	IOReturn err; // Errors from IO functions
	IOUSBFindInterfaceRequest request; // Interface Request
	io_iterator_t iter; // USB Interface iterator
	
	// Check that we actually have a command buffer
	if (buf==0)
		return K8055_NO_BUF;
    
	// Check address is valid
	if (address < 0 || address > 3)
		return K8055_BAD_ADDRESS;
	
	// Match device depending on card address
	
	switch (address)
	{
		case 0:
            matchUSBDevice (kK8055VendorId,kK8055Card0Id, &usbDevice);
            break;
            
		case 1:
            matchUSBDevice (kK8055VendorId,kK8055Card1Id, &usbDevice);
            break;
            
		case 2:
            matchUSBDevice (kK8055VendorId,kK8055Card2Id, &usbDevice);
            break;
            
		case 3:
            matchUSBDevice (kK8055VendorId,kK8055Card3Id, &usbDevice);
            break;
	}
	
	// Did we find it?
	
	if (!usbDevice)
	{
		return K8055_NOT_FOUND;
	}
	
	// At this point we need to get the interface to the device / configure the card / get the interface
    
	// Create the application interface to the USB device
    
	if (createUSBDeviceInterface(&usbDevice,&USBDeviceInterface))
    {
        return K8055_GENERAL_ERR;
    }
    
	// Attempt to open the device for exclusive access
	err = (*USBDeviceInterface)->USBDeviceOpen(USBDeviceInterface);
    
	if (err!=kIOReturnSuccess)
    {
        return K8055_OPEN_ERR;
    }
	
	USBDeviceOpen=TRUE;
    
	// Now Configure it - set appropriate configurations etc
    
	configureUSBDevice(); // Do any necessary configuration
	
	// Now get the interfaces
    
	request.bInterfaceClass = kUSBHIDInterfaceClass;
    request.bInterfaceSubClass = kIOUSBFindInterfaceDontCare;
    request.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;
    request.bAlternateSetting = kIOUSBFindInterfaceDontCare;
	
	err = (*USBDeviceInterface)->CreateInterfaceIterator(USBDeviceInterface, &request, &iter);
    
	if (err!=kIOReturnSuccess)
    {
        return K8055_GENERAL_ERR;
    }
    
	// Iterate over the matching interface - should only be 1 on this device
    
	usbInterface = IOIteratorNext(iter);
	
	if (!usbInterface)
    {
        return K8055_GENERAL_ERR;
    }
    
	// Create the application interface to the USB interface
    
	if (createUSBInterfaceInterface(&usbInterface,&USBInterfaceInterface))
    {
        return K8055_GENERAL_ERR;
    }
    
	// Open the device for exclusive access
	err = (*USBInterfaceInterface)->USBInterfaceOpen(USBInterfaceInterface);
    
	if (err!=kIOReturnSuccess)
    {
        return K8055_OPEN_ERR;
    }
    
	USBInterfaceOpen=TRUE;
    
	// Now just get a couple of bits of information
	
	// Fetch number of endpoints
	
	if ((*USBInterfaceInterface)->GetNumEndpoints(USBInterfaceInterface,&numEndpoints)!=kIOReturnSuccess)
    {
        return K8055_GENERAL_ERR;
    }
	
	int count=0;
	
	UInt8 direction;
    UInt8 number;
    UInt8 transferType;
    UInt16 maxPacketSize;
    UInt8 interval;
	
	// Fetch the pipe references for in and out
    
	for(count=1;count<=numEndpoints;count++)
    {
        (*USBInterfaceInterface)->GetPipeProperties (USBInterfaceInterface,count, &direction,&number, &transferType, &maxPacketSize, &interval);
        if ((direction==kUSBIn) && (transferType=kUSBInterrupt)) inPipeRef=count;
        if ((direction==kUSBOut) && (transferType=kUSBInterrupt)) outPipeRef=count;
    }
	
	if (!inPipeRef||!outPipeRef)
    {
        return K8055_GENERAL_ERR;
    }
    
    resetDevice(); // Initialise the device
    
    return K8055_OK;
    
}

void k8055::CloseDevice()
{
	resetDevice();
    
	// Call this function to close the open interfaces and delete all memory structures
	cleanup();
	
	// Reinitialise
	initialise();
	
}

// Reset the device

void k8055::resetDevice()
{
    
    if (USBInterfaceOpen)
        
    {
        buf[0]=0x00;
        buf[1]=0x00;
        buf[2]=0x00;
        buf[3]=0x00;
        buf[4]=0x00;
        buf[5]=0x00;
        buf[6]=0x00;
        buf[7]=0x00;
		
        (*USBInterfaceInterface)->WritePipe(USBInterfaceInterface,outPipeRef,buf,BUFSIZE);
        
    }
    
    ClearAllAnalog(); // Clear the analogue channels
    ClearAllDigital(); // Clear the digital outputs
    
    // ReadAnalogChannel(1); // Clear pipe
    
    readDevice(); // Read the inputs
    
}

// Read Analogue channel (channel number)

UInt32 k8055::ReadAnalogChannel (UInt8 channel)
{
    
    if (channel!=1 && channel!=2) return K8055_BAD_ADDRESS;
    
    readDevice();
    
    if (channel==1) return analogIn1 | K8055_OK;
    else return analogIn2 | K8055_OK;
    
}

// Read all Analogue channels

UInt32 k8055::ReadAllAnalog(UInt8 *channel1 , UInt8 *channel2)
{
    
    if (!USBInterfaceOpen) return K8055_OPEN_ERR;
    
    readDevice();
    
    *channel1=analogIn1;
    *channel2=analogIn2;
    
    return K8055_OK;
    
}

// Output analogue data to channel

UInt32 k8055::OutputAnalogChannel (UInt8 channel, UInt8 data)
{
    
    if (channel!=1 && channel!=2) return K8055_BAD_ADDRESS;
    
    if (channel==1) analogOut1=data;
    if (channel==2) analogOut2=data;
    
    return outputChannels();
    
}

// Output analogue data to both channels

UInt32 k8055::OutputAllAnalog(UInt8 channel1, UInt8 channel2)
{
	analogOut1=channel1;
	analogOut2=channel2;
	
	return outputChannels();
}

// Clear analogue channel

UInt32 k8055::ClearAnalogChannel (UInt8 channel)
{
    return OutputAnalogChannel(channel,K8055_CLEAR);
}

// Clear all analogue channels

UInt32 k8055::ClearAllAnalog()
{
	analogOut1=K8055_CLEAR;
	analogOut2=K8055_CLEAR;
	
	return outputChannels();
	
}

// Set analogue channel to all set

UInt32 k8055::SetAnalogChannel(UInt8 channel)
{
    
	return OutputAnalogChannel(channel,K8055_SET);
}

// set all analogue channels to all set

UInt32 k8055::SetAllAnalog()
{
	analogOut1=K8055_SET;
	analogOut2=K8055_SET;
	
	return outputChannels();
}

// Write all digital channels with data

UInt32 k8055::WriteAllDigital(UInt8 data)
{
    
    digitalOut=data;
    return outputChannels();
    
}

// Clear a digital channel

UInt32 k8055::ClearDigitalChannel(UInt8 channel)
{
    
    if (channel<1 || channel >8) return K8055_BAD_ADDRESS;
    
    channel--;
    
    return WriteAllDigital(digitalOut & ~(1 << channel));
    
}

// Clear all digital channels

UInt32 k8055::ClearAllDigital()
{
    return WriteAllDigital(K8055_CLEAR);
}

// Set a digital channel

UInt32 k8055::SetDigitalChannel(UInt8 channel)
{
    
    if (channel<1 || channel >8) return K8055_BAD_ADDRESS;
    
    channel--;
    
    return WriteAllDigital(digitalOut | (1 << channel));
    
}

// Set all digital channels

UInt32 k8055::SetAllDigital()
{
    return WriteAllDigital(K8055_SET);
}

// Read a digital channel

UInt32 k8055::ReadDigitalChannel(UInt8 channel)
{
    
    if (channel<1 || channel>5) return K8055_BAD_ADDRESS;
    
    channel--;
    
    return (ReadAllDigital() & (1 << channel));
    
}

// Read all digital channels

UInt32 k8055::ReadAllDigital()
{
    
    UInt8 channelstat=0;
    
    if (!USBInterfaceOpen) return K8055_OPEN_ERR;
    
    readDevice();
    
    if (digitalIn & K8055_SWITCH1) channelstat |= 1; // Switch 1
    if (digitalIn & K8055_SWITCH2) channelstat |= 2; // Switch 2
    if (digitalIn & K8055_SWITCH3) channelstat |= 4; // Switch 3
    if (digitalIn & K8055_SWITCH4) channelstat |= 8; // Switch 4
    if (digitalIn & K8055_SWITCH5) channelstat |= 16; // Switch 5
    
    return channelstat | K8055_OK;
    
}

// Reset internal counter

UInt32 k8055::ResetCounter(UInt8 counter)
{
    
    if (counter!=1 && counter!=2) return K8055_BAD_ADDRESS;
    if (!USBInterfaceOpen) return K8055_OPEN_ERR;
    
    buf[0]=0x02 + counter;
    buf[1]=0x00;
    buf[2]=0x00;
    buf[3]=0x00;
    buf[4]=0x00;
    buf[5]=0x00;
    buf[6]=0x00;
    buf[7]=0x00;
    
    (*USBInterfaceInterface)->WritePipe(USBInterfaceInterface,outPipeRef,buf,BUFSIZE);
    
    return K8055_OK;
    
}

// Read internal counter

UInt32 k8055::ReadCounter(UInt8 counter)
{
    
    
    if (counter!=1 && counter!=2) return K8055_BAD_ADDRESS;
    if (!USBInterfaceOpen) return K8055_OPEN_ERR;
    
    readDevice();
    
    if (counter==1) return (timer1 | K8055_OK);
    else return (timer2 | K8055_OK);
	
}

// Set Counter debounce time

UInt32 k8055::SetCounterDebounceTime(UInt8 counter, UInt16 DebounceTime)
{
    
    UInt8 dbt=0;
    
    if (counter!=1 && counter!=2) return K8055_BAD_ADDRESS;
    
    if (DebounceTime < 2)
		dbt = 1;
	else if (DebounceTime < 10)
		dbt = 3;
	else if (DebounceTime < 1000)
		dbt = 8;
	else if (DebounceTime < 5000)
		dbt = 88;
	else
		dbt = 255;
    
    buf[0]=0x00 + counter;
    buf[1]=0x00;
    buf[2]=0x00;
    buf[3]=0x00;
    buf[4]=0x00;
    buf[5]=dbt;
    buf[6]=dbt;
    buf[7]=0x00;
    
    (*USBInterfaceInterface)->WritePipe(USBInterfaceInterface,outPipeRef,buf,BUFSIZE);
    
    return K8055_OK;
}

// Read the device status

UInt32 k8055::readDevice()
{
    
    UInt32 bufsiz=BUFSIZE;
    
    if (!USBInterfaceOpen) return K8055_OPEN_ERR;
    
    UnsignedWide now;
    
    Microseconds(&now); // Get the current time
    
    // This logic is so we only read the device once every READ_INTERVAL and store the results
    
    if ( ((now.hi - lastread.hi) > 0) || ((now.lo - lastread.lo) > (1000*READ_INTERVAL)) )
	{
		lastread.hi=now.hi; // store the time
		lastread.lo=now.lo;
		
		// Now do the read and assign to the variables
		
		(*USBInterfaceInterface)->ReadPipe(USBInterfaceInterface,inPipeRef,buf,&bufsiz);
        
		digitalIn=buf[0];
		analogIn1=buf[2];
		analogIn2=buf[3];
		timer1=(buf[5]*256) + buf[4];
		timer2=(buf[7]*256) + buf[6];
	}
    
    return K8055_OK;
    
}

// Outputs channels

UInt32 k8055::outputChannels()
{
    
    if (USBInterfaceOpen)
        
    {
        
        buf[0]=0x05;
        buf[1]=digitalOut;
        buf[2]=analogOut1;
        buf[3]=analogOut2;
        buf[4]=0x00;
        buf[5]=0x00;
        buf[6]=0x00;
        buf[7]=0x00;
        
        (*USBInterfaceInterface)->WritePipe(USBInterfaceInterface,outPipeRef,buf,BUFSIZE);
        
        return K8055_OK;
        
    }
    
    else return K8055_OPEN_ERR;
    
}

// End k8055.cpp
