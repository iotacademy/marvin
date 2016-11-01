# Marvin Hardware
This overview contains documentation regarding the hardware used in Marvin. 

##General
The Marvin consists of 4 main features: A USB port, Arduino compatible microcontroller, 5 Grove connectors and a LoRa(WAN) modem. The board is optimized to be used for quick and easy applications in R&D and workshops, as well as a very reasonable solution for embedded applications.

##Grove connectors
The 5 grove connectors cover all analog(x2), digital(x2) and I2C(x1). 

![Grove connectors](https://raw.github.com/iotacademy/marvin/master/Hardware/Pictures/Grove.JPG)

##ISP
The 2x3 male header is used to burn the bootloader on the microcontroller. As this is done via SPI, this bus is available for other SPI peripherals. On the bottom there are an additional 6 contact pads using pogo pins instead of a 2x3 wired connector.

##Serial
The RX/TX pins from the microcontroller are available via an unpopulated connection. The LoRa modem is controlled over the same line, so it needs to be shut down when other UART devices are used to prevent errors.

##GPIO
Next to the Grove connectors there is another two unpopulated connection point, connected to GPIO 8 and 9 of the LoRa module.

##Antenna
The antenna is a 868MHz ceramic resonator antenna with a pigtail behind it. The choice to go for a ceramic antenna (instead of the regular antenna on SMA connector that is spec'd) was done for two reasons, the on-pcb ceramic antenna is a lot smaller and less fragile, and in our side-by-side tests the ceramic antenna performed considerably better on all scores.

##USB
The full size USB plug and the micro USB are linked directly together. The full size connector is used for plugging it right in your laptop, the micro USB can be used when the unit is embedded in a place where is not easily removeable and makes the system a lot more flexible.

##Dimensions
The overal dimensions of the PCB are 85x35mm and the pcb is 2.4mm thick. This thickness is required to make the PCB fit the usb port.

##BOM
algemene keuzes

##Schematics
LINK

##Board
LINK