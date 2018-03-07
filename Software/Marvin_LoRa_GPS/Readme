SETUP YOUR MARVIN + GPS 

The Microchip lora module is connected over UART with the Arduino Leonardo 32u4. 

Uart can only handle one master and one slave device. Since GPS is also using UART communcication, you have to kill the power to the RN2483.
Do this by writing pin 5 (power RN2483) and pin 6 (reset RN2483) LOW. 

Next you connect rx to tx of the gps and tx to rx, using the pads provided on the lower edge of the Marvin (USB pointing to left)

To control the power to the GPS, you need an PNP transistor that has the gate connected to one of the arduino's digital pin. 
In active mode, the GPS requires +50mA, which is more then a digital pin of the 32u4 can provide. 
Using a PNP between the VCC of the gps and the VCC of the marvin will allow for enough power to run the gps properly. 
You can use digital pin 12 for this, 3rd grove connector - look on the bottom of the board - to cut off the power. 

In this way, you can retreive gps coordinates, store them in a variable, kill gps power, fire up lora and send variables in the payload. 

Enjoy your gps lora tracker!
