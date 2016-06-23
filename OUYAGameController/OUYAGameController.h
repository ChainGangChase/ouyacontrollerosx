//
// OUYAGameController.h
//
// Copyright (c) 2013 Edmund Kapusniak
//
// Uses code from hid-ouya.c HID driver for OUYA Game Controller(s)
// Copyright (c) 2013 OUYA
//

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */


#include <IOKit/bluetooth/IOBluetoothHIDDriver.h>


#define OUYAGameController com_birotanker_OUYAGameController


class OUYAGameController : public IOBluetoothHIDDriver
{
    OSDeclareDefaultStructors( OUYAGameController )


public:

    // Lifetime.

    virtual bool init( OSDictionary * dictionary = 0 );
    virtual bool handleStart( IOService * provider );
    virtual void handleStop( IOService *  provider );


    // Overridden to ensure the OUYA controller is treated as a gamepad.

    virtual OSNumber* newPrimaryUsageNumber() const;
    virtual OSNumber* newPrimaryUsagePageNumber() const;

    
    // Overridden to implement touchpad fixup.

    virtual IOReturn newReportDescriptor( IOMemoryDescriptor** descriptor ) const;


protected:

    // Lifetime.
    
    virtual void free();


private:

    const uint8_t* _hrdData;
    size_t _hrdSize;


};


