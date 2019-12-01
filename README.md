# SJ2WPTTIndicator

This is a QT software that monitors network traffic from DXLog or N1MM and indicates if a radio is transmitting.

To compile it download the latest version of QT with the minGW compiler. When this is installed start the QT command prompt and go the the root directory of SJ2WPTTIndicator.
qmake -config release
make

Move the leds directory and the settings.ini to the same path as the binary generated when compiling.

The default setup was made for SJ2W where we have four positions. To modify the software to have more positions one would need to modify the GUI and some slight changes in the code.

Station1NetworkName=INBAND
This is the name of the computer on the Network. This is the Windows network ID.

Station1DisplayName=Inband
This is the name that gets displayed on the GUI

DXLog
-----
To configure it for DXLog check the settings.ini and change the LogSoftware to DXLog and the Port to 9888.
In DXLog the software monitors the Interlock traffic between the computers via UDP so interlock need to be enabled for this to work.

N1MM
----
To configure it for N1MM check the settings.ini and change the LogSoftware to N1MM and the Port to 12060.

