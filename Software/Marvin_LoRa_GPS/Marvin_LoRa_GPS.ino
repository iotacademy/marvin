/*
  Marvin_LoRa_GPS
  
  Connect the gps using a grove - female jumper wire:
  - GND to any GND
  - VCC to port D3
  - RX  to TX - located on the under edge of the board. 
  - TX to RX 

  This version supports:
  - Sending LoRa uplink messages using ABP that are given as input from the serial port on your laptop
  - Blink three times when sending data
  - Power control to RN2903 module

  Instructions:
  - Get the latest version of the Arduino software
  - In Arduino IDE select Arduino Leonardo and com port of your device
  - Please adjust ABP adresses and key below to match yours
*/
// Port to assign the type of lora data (any port can be used between 1 and 223)
int     set_port  = 1;

// Some standard ports that depend on the layout of the Marvin
long    defaultBaudRate = 9600;
long    newBaudRate = 57600;
int     reset_port = 5;
int     RN2903_power_port = 6;
int     led_port = 13;
int     buttonstate = A3;
int     gps_pwr = 3;
float   latti;
float   longi;
int     counter = 0;
int     powerstategps = 0;
int     waarde;

// Some global items
String reader = "";
#include <TinyGPS++.h>  //you will have to download+install the tinygps++ library, just google it, first hit.
#include <math.h>
#include <MarvinLib.h>

TinyGPSPlus gps;
marvin::LoRaCom loraModule;

void setup()
{
  pinMode(gps_pwr, OUTPUT);
  pinMode(buttonstate, INPUT);
  digitalWrite(gps_pwr, LOW);
  loraModule.initializeLoRa();
  delay(100);
  Serial.println("Hello There, Let's go!");
  pinMode(led_port, OUTPUT); // Initialize LED port
  blinky();
}

void loop(){

// This program assumes you use a button on the A3 port. Each button press the program below will run. 

  if (digitalRead(buttonstate) == 1 && powerstategps == 0){
      loraModule.powerOn();
      loraModule.exec_general_LoRa_command("mac tx uncnf " + String(set_port) + String(" ") + 01);                 //send confirmation message
      blinky();
      delay(1000);
      Serial.println("Zeroing in on your location, just a sec...");      //start gps tracking
      loraModule.powerOff();                                             //kill rn module for uart
      delay(100);
      digitalWrite(gps_pwr, HIGH);                                                              //start gps on port gps_pwr
      powerstategps = 1;                                                                        //write variable to indicate gps is on
      Serial.println("GPS Powered up");
  }
  
  if (digitalRead(buttonstate) == 1 && powerstategps == 1){
    digitalWrite(gps_pwr, LOW);
    Serial.println("GPS Killed!");
    powerstategps = 0;
    delay(2000);
  }
  
  if (digitalRead(gps_pwr) == HIGH){
        printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);                        //start printing values - will return stars as long as there is no valid location
        printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
        Serial.println(" ");
        smartDelay(1000);

        if (millis() > 5000 && gps.charsProcessed() < 10) {
            Serial.println(F("No GPS data received: check wiring"));
            blinky_error();
        }
  
        if(gps.location.isValid() == true && gps.location.isUpdated() == true){
          digitalWrite(13, HIGH);

          latti = gps.location.lat();
          longi = gps.location.lng();        
          uint32_t  n = latti * 1000000;
          uint32_t  e = longi * 1000000;
          Serial.println(n);
          Serial.println(e);  
          blinky_error();
          digitalWrite(gps_pwr, LOW); //kill gps
          powerstategps = 0;
          Serial.println("GPS KILLED");
          Serial.println("sending gps now");
          
          loraModule.powerOn();
          digitalWrite(led_port, HIGH);
          loraModule.exec_general_LoRa_command("mac tx uncnf " + String(set_port) + String(" ") + String(n) + String(e));
          blinky();
          delay(1000);
          loraModule.powerOff();
          digitalWrite(led_port, LOW);
          
        }
    }
// go back to main loop again and wait for gps to become true
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
void blinky_error()
{
  for (int i = 0; i < 4; ++i)
  {
    digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);              // wait for a second
    digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
    delay(50);
  }
}
//---------------------end lora part

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}



