## OUYA controller driver for OSX

The OUYA controller is a Bluetooth device with an integrated touchpad.  My Mac speaks Bluetooth,
but unfortunately decided that the controller must be a mouse.  And then got the analog stick and
the mouse pointer inputs confused. Whoops!

So I spent a day or so delving into IOKit and writing a very simple driver. To my surprise
just loading a driver other than the standard one seemed to fix the pointer jumping problem.
To make sure, the driver reports the device as a gamepad instead of a mouse.

This version of the driver also:
* Based on the Linux kernel code released by OUYA, provides an updated HID report descriptor for
controllers of version 1.04 or later.  This is untested as I don't have access to such a controller.
* Fixes the encoding of the HID report descriptor for controller version 1.03 and below.  The 65535
maximum axis range is encoded incorrectly by the descriptor reported by the controller itself.
* Reorganises the button usage numbers to match the button usages reported by a USB Xbox 360 controller.
I am not sure how useful this is.


### System Integrity Protection

The kext is not signed, so to use it on OSX 10.10 and later you will have to disable System
Integrity Protection using `csrutil` as described by Apple at [Configuring System Integrity Protection](https://developer.apple.com/library/mac/documentation/Security/Conceptual/System_Integrity_Protection_Guide/ConfiguringSystemIntegrityProtection/ConfiguringSystemIntegrityProtection.html).
I do not recommend doing so and take no responsiblity for any security problems or system
issues you may encounter.


### License

As it is derived from code contributed by OUYA to the Linux kernel, this driver is covered by the
GNU General Public License, version 2


### NO WARRANTY

  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.


