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


//*** Set parameters here BEGIN ---->
String  set_nwkskey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // Put your 32 hex char here
String  set_appskey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // Put your 32 hex char here
String  set_devaddr = "xxxxxxxx"; // Put your 8 hex char here
//*** <---- END Set parameters here

// Some global items
String reader = "";
#include <TinyGPS++.h>  //you will have to download+install the tinygps++ library, just google it, first hit.
#include <math.h>
TinyGPSPlus gps;


void setup()
{
  pinMode(gps_pwr, OUTPUT);
  pinMode(buttonstate, INPUT);
  digitalWrite(gps_pwr, LOW);
  initializeRN2903(RN2903_power_port, reset_port);
  delay(100);
  Serial.println("Hello There, Let's go!");
  pinMode(led_port, OUTPUT); // Initialize LED port
  blinky();
}

void loop(){

// This program assumes you use a button on the A3 port. Each button press the program below will run. 

  if (digitalRead(buttonstate) == 1 && powerstategps == 0){
      restartRN2903(RN2903_power_port, reset_port);
      send_LoRa_Command("mac tx uncnf " + String(set_port) + String(" ") + 01);                 //send confirmation message
      delay(1000);
      read_data_from_LoRa_Mod();
      blinky();
      delay(1000);
      Serial.println("Zeroing in on your location, just a sec...");      //start gps tracking
      killRN2903(RN2903_power_port, reset_port);                                                //kill rn module for uart
      delay(100);
      digitalWrite(gps_pwr, HIGH);                                                              //start gps on port gps_pwr
      powerstategps = 1;                                                                        //write variable to indicate gps is on
      print_to_console("GPS Powered up");
  }
  
   if (digitalRead(buttonstate) == 1 && powerstategps == 1){
      digitalWrite(gps_pwr, LOW);
      print_to_console("GPS Killed!");
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
                    
                    restartRN2903(RN2903_power_port, reset_port);
                    digitalWrite(led_port, HIGH);
                    send_LoRa_Command("mac tx uncnf " + String(set_port) + String(" ") + String(n) + String(e));
                    delay(1000);
                    read_data_from_LoRa_Mod();
                    blinky();
                    delay(1000);
                    killRN2903(RN2903_power_port, reset_port);
                    digitalWrite(led_port, LOW);
                    
        }
    }
// go back to main loop again and wait for gps to become true
}


//Lora part --------------------------------------------

void InitializeSerials(long baudrate)
{
  Serial.flush ();   // wait for send buffer to empty
  delay (100);    // let last character be sent
  Serial1.flush ();   // wait for send buffer to empty
  delay (100);    // let last character be sent
  Serial.end ();
  Serial1.end ();
  delay(100);
  Serial.begin(baudrate);
  Serial1.begin(baudrate);
  delay(100);
  print_to_console("Serial ports initialised");
}

void initializeRN2903(int pwr_port, int rst_port)
{
  //Enable power to the RN2903
  InitializeSerials(newBaudRate);
  delay(1000);
  pinMode(pwr_port, OUTPUT);
  digitalWrite(pwr_port, HIGH);
  delay(1000);
  print_to_console("RN2903 Powered up");

  //Disable reset pin
  pinMode(rst_port, OUTPUT);
  digitalWrite(rst_port, HIGH);

  //Configure LoRa module
  send_LoRa_Command("sys reset");
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set nwkskey " + set_nwkskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set appskey " + set_appskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set devaddr " + set_devaddr);
  read_data_from_LoRa_Mod();

  //For this commands some extra delay is needed.
  send_LoRa_Command("mac set adr on");
  //send_LoRa_Command("mac set dr 0"); //uncomment this line to fix the RN2903 on SF12 (dr=DataRate)
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac save");
  delay(1000);
  read_data_from_LoRa_Mod();

}

void killRN2903(int pwr_port, int rst_port)
{
  //disable power to the RN2903
 // pinMode(pwr_port, OUTPUT);
 // digitalWrite(pwr_port, LOW);
  //Disable reset pin
  pinMode(rst_port, OUTPUT);
  digitalWrite(rst_port, LOW);
  print_to_console("RN2903 killed");
  delay(1000);
  InitializeSerials(defaultBaudRate);
}

void restartRN2903(int pwr_port, int rst_port)
{
  //Enable power to the RN2903
  InitializeSerials(newBaudRate);
  delay(1000);
  //Disable reset pin
  pinMode(rst_port, OUTPUT);
  digitalWrite(rst_port, HIGH);
  pinMode(pwr_port, OUTPUT);
  digitalWrite(pwr_port, HIGH);
  delay(1000);
  print_to_console("RN2903 repowered up");
  
  send_LoRa_Command("mac join abp");
  delay(1000);
  read_data_from_LoRa_Mod();
  send_LoRa_Command("radio set crc off");
  delay(1000);
  read_data_from_LoRa_Mod();
  delay(100);
}

void print_to_console(String message)
{
  Serial.println(message);
}

void read_data_from_LoRa_Mod()
{
  if (Serial1.available()) {
    String inByte = Serial1.readString();
    delay(100);
    Serial.println(inByte);
  }

}

void send_LoRa_Command(String cmd)
{
  print_to_console("Now sending: " + cmd);
  Serial1.println(cmd);
  delay(500);
}

void send_LoRa_data(int tx_port, String rawdata)
{
  send_LoRa_Command("mac tx uncnf " + String(tx_port) + String(" ") + rawdata);
  delay(100);
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
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);              // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(50);              // wait for a second
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);              // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(50);              // wait for a second
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);              // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(50);              // wait for a second
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);              // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(50);              // wait for a second

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



