# Marvin
Essentials for Marvin LoRa Development

## Getting started

### Install Arduino IDE

Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software) and follow the installer. 

If you want to donate to Arduino -to help developing their software we are thankfully using- this is possible, but not mandatory.

### Connect Marvin to computer

Use your left or right (or a combination) hand to push the Marvin in the USB port of your computer. Your computer will quickly befriend it and say "blingBLING", which is computer language for "we are now BFFs!".

### Upload Blinky

Download [Blinky.ino](https://github.com/iotacademy/marvin/blob/master/Software/MarvinBlink/MarvinBlink.ino) and open it in the Arduino IDE.

From the 'Tools' menu select 'Arduino Leonardo' in the 'Boards' section.

Click upload to send the software to the Marvin. If all went well it will say Uploading complete and the second LED will blink, if not try uploading again.

![Board selection](https://raw.github.com/iotacademy/marvin/master/Software/Pictures/Blinky.jpg)

![Upload sketch](https://raw.github.com/iotacademy/marvin/master/Software/Pictures/Upload.jpg)

### Check RN2483

Download [MarvinMultiSerial.ino](https://github.com/iotacademy/marvin/blob/master/Software/MarvinMultiSerial/MarvinMultiSerial.ino) and open it in the Arduino IDE.

Upload it to the Marvin and then open the Serial Monitor

![Serial Monitor](https://raw.github.com/iotacademy/marvin/master/Software/Pictures/Serial.jpg)

Set the baudrate to 57600 (same as in the sketch) and select 'CR+NL'. Now you can directly speak to the RN2483 and get a response.

If you for instance send 'sys get ver', the module will return its firmware version back. This will be 1.0.1 with some historic date.

Possible other commands can be find in the module [data sheet](http://ww1.microchip.com/downloads/en/DeviceDoc/40001784E.pdf) or in [one of the many](https://www.disk91.com/2015/technology/networks/first-step-in-lora-land-microchip-rn2483-test/) [examples](https://thinginnovations.uk/getting-started-with-microchip-rn2483-lorawan-modules) [by others](http://kotahi.net/configure-microchip-rn2483-module/).

## Using your first sensor

### Sensor and connector

### Digital sensors

### Analog sensors

### Libraries for I2C sensors

## Sending message to other Marvin

### Broadcasting

### Listening

## Connecting to KPN LoRa network

## Connecting to The Things Network

## Power management
