# Marvin Hardware
This overview contains documentation regarding the hardware used in Marvin. 

##General
The Marvin consists of 4 main features: A USB port, Arduino compatible microcontroller, 5 Grove connectors and a LoRa(WAN) modem. The board is optimized to be used for quick and easy applications in R&D and workshops, as well as a very reasonable solution for embedded applications.

##ATMEGA32u4

![ATMEGA32u4](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/ATMEGA32u4.JPG)

##Grove connectors
The 5 grove connectors cover all analog(x3), digital(x1) and I2C(x1). The I2C port can also be used as a digital input/output.

![Grove connectors](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/Grove.JPG)

##ISP
The 2x3 male header is used to burn the bootloader on the microcontroller. As this is done via SPI, this bus is available for other SPI peripherals. On the bottom there are an additional 6 contact pads using pogo pins instead of a 2x3 wired connector.

![ISP header](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/ISPheader.JPG)

##Serial
The RX/TX pins from the microcontroller are available via an unpopulated connection. The LoRa modem is controlled over the same line, so it needs to be shut down when other UART devices are used to prevent errors.

##GPIO
Next to the Grove connectors there is another two unpopulated connection point, connected to GPIO 8 and 9 of the LoRa module.

![GPIO](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/RN2483GPIO.JPG)

##Antenna
The antenna is a 868MHz ceramic resonator antenna with a pigtail behind it. The choice to go for a ceramic antenna (instead of the regular antenna on SMA connector that is spec'd) was done for two reasons, the on-pcb ceramic antenna is a lot smaller and less fragile, and in our side-by-side tests the ceramic antenna performed considerably better on all scores.

![Antenna](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/Antenna.JPG)

##USB
The full size USB plug and the micro USB are linked directly together. The full size connector is used for plugging it right in your laptop, the micro USB can be used when the unit is embedded in a place where is not easily removeable and makes the system a lot more flexible.

##Level shifters
The microcontroller and the LoRa modem work at different voltages (5v&3.3V), so for the communication and reset lines there are level shifters in place. 

![Level Shifters](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/LevelShifters.JPG)

##Dimensions
The overal dimensions of the PCB are 85x30mm and the pcb is 2.4mm thick. This thickness is required to make the PCB fit the usb port.

![Dimensions](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/Dimensions.JPG)

##Board and Schematics
See Layout folder.