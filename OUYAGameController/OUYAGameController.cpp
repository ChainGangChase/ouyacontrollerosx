//
// OUYAGameController.cpp
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


#include "OUYAGameController.h"


OSDefineMetaClassAndStructors( OUYAGameController, IOBluetoothHIDDriver )


/*
    Xbox 360 controller:
    
        axis Z      left trigger
        axis Rz     right trigger
        axis XY     left thumbstick
        axis RxRy   right thumbstick
        button C    UP
        button D    DOWN
        button E    LEFT
        button F    RIGHT
        button 9    START
        button A    BACK
        button 7    left stick click
        button 8    right stick click
        button 5    LB
        button 6    RB
        button B    XBOX
                    -
        button 1    A
        button 2    B
        button 3    X
        button 4    Y
*/


/*
    OUYA Controller v1.03
 
    report 7 (gamepad)
    {
        int16_t xy[ 2 ];
        int16_t rxy[ 2 ];
        int16_t z;
        int16_t rz;
        uint16_t buttons; // O U Y A LB RB LS RS U D L R LT RT centralpress centralhold
        output uint8_t unknown[ 7 ]
    }

    report 6 (mouse)
    {
        uint8_t mousebuttons; // 1-3
        int8_t mousexy[ 2 ];
        int8_t mousewheel;
    }
 
    report 3 (battery)
    {
        uint8_t battery;
        uint8_t unknown[ 6 ];
    }
 
    report 255
    {
        uint8_t unknown;
    }
 
*/


static uint8_t ouya_rdesc_103[] = {

    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x05,        // Usage (Game Pad)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x07,        //   Report ID (7)
    0xA1, 0x00,        //   Collection (Physical)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x15, 0x00,                     //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00,   //     Logical Maximum (65535)
    0x35, 0x00,                     //     Physical Minimum (0)
    0x47, 0xFF, 0xFF, 0x00, 0x00,   //     Physical Maximum (65535)
    0x95, 0x02,        //     Report Count (2)
    0x75, 0x10,        //     Report Size (16)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xA1, 0x00,        //   Collection (Physical)
    0x09, 0x33,        //     Usage (Rx)
    0x09, 0x34,        //     Usage (Ry)
    0x15, 0x00,                     //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00,   //     Logical Maximum (65535)
    0x35, 0x00,                     //     Physical Minimum (0)
    0x47, 0xFF, 0xFF, 0x00, 0x00,   //     Physical Maximum (65535)
    0x95, 0x02,        //     Report Count (2)
    0x75, 0x10,        //     Report Size (16)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xA1, 0x00,        //   Collection (Physical)
    0x09, 0x32,        //     Usage (Z)
    0x15, 0x00,                     //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00,   //     Logical Maximum (65535)
    0x35, 0x00,                     //     Physical Minimum (0)
    0x47, 0xFF, 0xFF, 0x00, 0x00,   //     Physical Maximum (32767)
    0x95, 0x01,        //     Report Count (1)
    0x75, 0x10,        //     Report Size (16)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xA1, 0x00,        //   Collection (Physical)
    0x09, 0x35,        //     Usage (Rz)
    0x15, 0x00,                     //     Logical Minimum (0)
    0x27, 0xFF, 0xFF, 0x00, 0x00,   //     Logical Maximum (65535)
    0x35, 0x00,                     //     Physical Minimum (0)
    0x47, 0xFF, 0xFF, 0x00, 0x00,   //     Physical Maximum (65535)
    0x95, 0x01,        //     Report Count (1)
    0x75, 0x10,        //     Report Size (16)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0x05, 0x09,        //   Usage Page (Button)
    0x09, 0x01,        //   Usage (0x01)
    0x09, 0x03,        //   Usage (0x03)
    0x09, 0x04,        //   Usage (0x04)
    0x09, 0x02,        //   Usage (0x02)
    0x09, 0x05,        //   Usage (0x05)
    0x09, 0x06,        //   Usage (0x06)
    0x09, 0x07,        //   Usage (0x07)
    0x09, 0x08,        //   Usage (0x08)
    0x09, 0x0C,        //   Usage (0x0C)
    0x09, 0x0D,        //   Usage (0x0D)
    0x09, 0x0E,        //   Usage (0x0E)
    0x09, 0x0F,        //   Usage (0x0F)
    0x09, 0x10,        //   Usage (0x10)
    0x09, 0x11,        //   Usage (0x11)
    0x09, 0x0B,        //   Usage (0x0B)
    0x09, 0x12,        //   Usage (0x12)
    0x95, 0x10,        //   Report Count (16)
    0x75, 0x01,        //   Report Size (1)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
    0xA1, 0x02,        //   Collection (Logical)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x07,        //     Report Count (7)
    0x46, 0xFF, 0x00,  //     Physical Maximum (255)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x09, 0x01,        //     Usage (0x01)
    0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              //   End Collection
    0xC0,              // End Collection
    
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02,        // Usage (Mouse)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x06,        //   Report ID (6)
    0x09, 0x01,        //   Usage (Pointer)
    0xA1, 0x00,        //   Collection (Physical)
    0x05, 0x09,        //     Usage Page (Button)
    0x19, 0x01,        //     Usage Minimum (0x01)
    0x29, 0x03,        //     Usage Maximum (0x03)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x03,        //     Report Count (3)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x05,        //     Report Size (5)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x02,        //     Report Count (2)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x38,        //     Usage (Wheel)
    0x15, 0x81,        //     Logical Minimum (-127)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xC0,              // End Collection

    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x03,        //   Report ID (3)
    0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,        //   Usage (Keyboard)
    0xA1, 0x02,        //   Collection (Logical)
    0x05, 0x06,        //     Usage Page (Generic Dev Ctrls)
    0x09, 0x20,        //     Usage (Battery Strength)
    0x15, 0x00,        //     Logical Minimum (0)
    0x26, 0xFF, 0x00,  //     Logical Maximum (255)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x06, 0xBC, 0xFF,  //     Usage Page (Vendor Defined 0xFFBC)
    0x0A, 0xAD, 0xBD,  //     Usage (0xBDAD)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x06,        //     Report Count (6)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0xC0,              // End Collection

    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0xFF,        //   Report ID (255)
    0x05, 0x06,        //   Usage Page (Generic Dev Ctrls)
    0x95, 0x01,        //   Report Count (1)
    0x75, 0x02,        //   Report Size (2)
    0x19, 0x24,        //   Usage Minimum (0x24)
    0x29, 0x26,        //   Usage Maximum (0x26)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x06,        //   Report Size (6)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection

};




/*
    OUYA Controller v1.04 or later (untested)
    
    report 7
    {
        uint8_t xy[ 2 ];
        uint8_t rxy[ 2 ];
        uint8_t z;
        uint8_t rz;
        uint16_t buttons;
        uint8_t mousebuttons; // 1-3
        int8_t mousexy[ 2 ]
        output uint8_t unknown[ 7 ]
    }
    
    report 3 (battery)
    {
        uint8_t battery;
        uint8_t unknown[ 6 ];
    }
 
*/


static uint8_t ouya_rdesc_fixed[] = {

    0x05, 0x01,         /*  Usage Page (Desktop),           */
    0x09, 0x05,         /*  Usage (Game Pad),               */

    0xA1, 0x01,         /*  Collection (Application),       */
    0x85, 0x07,         /*      Report ID (7),              */

    0xA1, 0x00,         /*      Collection (Physical),      */
    0x09, 0x30,         /*          Usage (X),              */
    0x09, 0x31,         /*          Usage (Y),              */
    0x15, 0x00,         /*          Logical Minimum (0),    */
    0x26, 0xFF, 0x00,   /*          Logical Maximum (255),  */
    0x35, 0x00,         /*          Physical Minimum (0),   */
    0x46, 0xFF, 0x00,   /*          Physical Maximum (255), */
    0x95, 0x02,         /*          Report Count (2),       */
    0x75, 0x08,         /*          Report Size (8),        */
    0x81, 0x02,         /*          Input (Variable),       */
    0xC0,               /*      End Collection,             */

    0xA1, 0x00,         /*      Collection (Physical),      */
    0x09, 0x33,         /*          Usage (Rx),             */
    0x09, 0x34,         /*          Usage (Ry),             */
    0x15, 0x81,         /*          Logical Minimum (0),    */
    0x26, 0xFF, 0x00,   /*          Logical Maximum (255),  */
    0x35, 0x00,         /*          Physical Minimum (0),   */
    0x46, 0xFF, 0x00,   /*          Physical Maximum (255), */
    0x95, 0x02,         /*          Report Count (2),       */
    0x75, 0x08,         /*          Report Size (8),        */
    0x81, 0x02,         /*          Input (Variable),       */
    0xC0,               /*      End Collection,             */

    0xA1, 0x00,         /*      Collection (Physical),      */
    0x09, 0x32,         /*          Usage (Z),              */
    0x15, 0x00,         /*          Logical Minimum (0),    */
    0x26, 0xFF, 0x00,   /*          Logical Maximum (255),  */
    0x35, 0x00,         /*          Physical Minimum (0),   */
    0x46, 0xFF, 0x00,   /*          Physical Maximum (255), */
    0x95, 0x01,         /*          Report Count (1),       */
    0x75, 0x08,         /*          Report Size (8),        */
    0x81, 0x02,         /*          Input (Variable),       */
    0xC0,               /*      End Collection,             */

    0xA1, 0x00,         /*      Collection (Physical),      */
    0x09, 0x35,         /*          Usage (Rz),             */
    0x15, 0x00,         /*          Logical Minimum (0),    */
    0x26, 0xFF, 0x00,   /*          Logical Maximum (255),  */
    0x35, 0x00,         /*          Physical Minimum (0),   */
    0x46, 0xFF, 0x00,   /*          Physical Maximum (255), */
    0x95, 0x01,         /*          Report Count (1),       */
    0x75, 0x08,         /*          Report Size (8),        */
    0x81, 0x02,         /*          Input (Variable),       */
    0xC0,               /*      End Collection,             */

    0x05, 0x09,         /*      Usage Page (Button),        */
    0x19, 0x01,         /*      Usage Minimum (01h),        */
    0x29, 0x10,         /*      Usage Maximum (10h),        */
    0x95, 0x10,         /*      Report Count (16),          */
    0x75, 0x01,         /*      Report Size (1),            */
    0x81, 0x02,         /*      Input (Variable),           */

    /*   ORIGINAL REPORT DESCRIPTOR FOR TOUCHPAD INPUT  */
    /* 06 00 ff a1 02 09 02 15 00 26 ff 00 35 00 46 ff 00 95 03 75 08 81 02 c0 */

    0x06, 0x00, 0xFF,   /*      Usage Page (Custom),        */
    0x09, 0x02,         /*      Usage (Mouse),              */
    0x09, 0x01,         /*      Usage (Pointer),            */
    0xA1, 0x00,         /*      Collection (Physical),      */
    0x05, 0x09,         /*          Usage Page (Button),    */
    0x19, 0x01,         /*          Usage Minimum (01h),    */
    0x29, 0x03,         /*          Usage Maximum (03h),    */
    0x15, 0x00,         /*          Logical Minimum (0),    */
    0x25, 0x01,         /*          Logical Maximum (1),    */
    0x95, 0x03,         /*          Report Count (3),       */
    0x75, 0x01,         /*          Report Size (1),        */
    0x81, 0x02,         /*          Input (Variable),       */
    0x95, 0x01,         /*          Report Count (1),       */
    0x75, 0x05,         /*          Report Size (5),        */
    0x81, 0x01,         /*          Input (Constant),       */
    0x05, 0x01,         /*          Usage Page (Desktop),   */
    0x09, 0x30,         /*          Usage (X),              */
    0x09, 0x31,         /*          Usage (Y),              */
    0x15, 0x81,         /*          Logical Minimum (-127), */
    0x25, 0x7f,         /*          Logical Maximum (127),  */
    0x95, 0x02,         /*          Report Count (2),       */
    0x75, 0x08,         /*          Report Size (8),        */
    0x81, 0x06,         /*          Input (Relative),       */
    0xC0,               /*      End Collection,             */

    0x06, 0x00, 0xFF,   /*      Usage Page (Custom),        */
    0xA1, 0x02,         /*      Collection (Logical),       */
    0x75, 0x08,         /*          Report Size (8),        */
    0x95, 0x07,         /*          Report Count (7),       */
    0x46, 0xFF, 0x00,   /*          Physical Maximum (255), */
    0x26, 0xFF, 0x00,   /*          Logical Maximum (255),  */
    0x09, 0x01,         /*          Usage (Pointer),        */
    0x91, 0x02,         /*          Output (Variable),      */
    0xC0,               /*      End Collection,             */

    0xC0,               /*  End Collection                  */


    0x06, 0x00, 0xFF,   /*  Usage Page (Custom),            */
    0x05, 0x0C,         /*  Usage Page (Consumer),          */
    0x09, 0x01,         /*  Usage (Consumer Control),       */

    0xA1, 0x01,         /*  Collection (Application),       */
    0x85, 0x03,         /*      Report ID (3),              */
    0x05, 0x01,         /*      Usage Page (Desktop),       */
    0x09, 0x06,         /*      Usage (Keyboard),           */
    0xA1, 0x02,         /*      Collection (Logical),       */
    0x05, 0x06,         /*          Usage Page (Generic),   */
    0x09, 0x20,         /*          Usage (Battery Strgth), */
    0x15, 0x00,         /*          Logical Minimum (0),    */
    0x26, 0xFF, 0x00,   /*          Logical Maximum (255),  */
    0x75, 0x08,         /*          Report Size (8),        */
    0x95, 0x01,         /*          Report Count (1),       */
    0x81, 0x02,         /*          Input (Variable),       */
    0x06, 0xBC, 0xFF,   /*          Usage Page (Custom),    */

    0x0A, 0xAD, 0xBD,   /*  UNKNOWN */

    0x75, 0x08,         /*          Report Size (8),        */
    0x95, 0x06,         /*          Report Count (6),       */
    0x81, 0x02,         /*          Input (Variable),       */
    0xC0,               /*      End Collection,             */

    0xC0,               /*  End Collection                  */

};



// Lifetime.

bool OUYAGameController::init( OSDictionary* dictionary )
{
    if ( ! IOBluetoothHIDDriver::init( dictionary ) )
        return false;
    
    _hrdData = NULL;
    _hrdSize = 0;
    return true;
}

bool OUYAGameController::handleStart( IOService* provider )
{
    if ( ! IOBluetoothHIDDriver::handleStart( provider ) )
        return false;

    // Get version number of controller hardware.
    OSNumber* version = this->newVersionNumber();
    uint16_t maj = version->unsigned16BitValue() >> 8 & 0xFF;
    uint16_t min = version->unsigned16BitValue() & 0xFF;
    version->release();
    
    // Check if touchpad fixup is required.
    if ( ( maj == 0x01 && min >= 0x04 ) || ( maj == 0xE1 && min >= 0x3A ) )
    {
        _hrdData = ouya_rdesc_fixed;
        _hrdSize = sizeof( ouya_rdesc_fixed );
    }
    else
    {
        _hrdData = ouya_rdesc_103;
        _hrdSize = sizeof( ouya_rdesc_103 );
    }
    
    return true;
}

void OUYAGameController::handleStop( IOService* provider )
{
    IOBluetoothHIDDriver::handleStop( provider );
}

void OUYAGameController::free()
{
    IOBluetoothHIDDriver::free();
}




// Overridden to ensure the OUYA controller is treated as a gamepad.

OSNumber* OUYAGameController::newPrimaryUsageNumber() const
{
    return OSNumber::withNumber( 0x0005 /* Game Pad */, 32 );
}

OSNumber* OUYAGameController::newPrimaryUsagePageNumber() const
{
    return OSNumber::withNumber( 0x0001 /* Generic Desktop */, 32 );
}



// Overridden to implement touchpad fixup.

IOReturn OUYAGameController::newReportDescriptor( IOMemoryDescriptor** descriptor ) const
{
    if ( _hrdData )
    {
        *descriptor = IOBufferMemoryDescriptor::withBytes
        (
            _hrdData,
            _hrdSize,
            kIODirectionInOut
        );
        if ( *descriptor )
            return kIOReturnSuccess;
        else
            return kIOReturnNoMemory;
    }
    else
    {
        return IOBluetoothHIDDriver::newReportDescriptor( descriptor );
    }
}


