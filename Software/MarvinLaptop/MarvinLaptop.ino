/*
MarvinLaptop

Basic controls of IoT Academy Marvin LoRa Development board through your Laptop

This version supports:
	- Sending LoRa uplink messages using ABP that are given as input from the serial port on your laptop
	- Blink three times when sending data
  - Power control to RN2483 module

Instructions:
	- Get the latest version of the Arduino software
	- In Arduino IDE select Arduino Leonardo and com port of your device
	- Please adjust ABP adresses and key below to match yours
*/
// Port to assign the type of lora data (any port can be used between 1 and 223)
int     set_port  = 1;

// Some standard ports that depend on the layout of the Marvin
long    defaultBaudRate = 57600;
int     reset_port = 5;
int     RN2483_power_port = 6;
int     led_port = 13;

#include <MarvinLib.h>

// Some global items
String reader = "";

marvin::LoRaCom loraModule;

/*
 * Setup() function is called when board is started. Marvin uses a serial connection to talk to your pc and a serial
 * connection to talk to the RN2483, these are both initialized in seperate functions. Also some Arduino port are 
 * initialized and a LED is called to blink when everything is done. 
 */
void setup() {
  InitializeSerials(defaultBaudRate);
  loraModule.initializeLoRa();
  pinMode(led_port, OUTPUT); // Initialize LED port  
  blinky();
  Serial.print("Payload?");
}

void loop() {
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      reader += c; //makes the string readString
    } 
  }

  if (reader.length() >0) {
    reader.replace("\r","");
    reader.replace("\n","");
    Serial.print("Read: " + reader);
    delay(1000);
    loraModule.sendData(set_port, reader);
    blinky();
    reader = "";
    delay(1000);
    Serial.print("Payload?");
  }
}

void InitializeSerials(long baudrate)
{
  Serial.begin(baudrate);
  Serial1.begin(baudrate);
  delay(1000);
  Serial.print("Serial ports initialised");
}

void blinky()
{
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second

}


