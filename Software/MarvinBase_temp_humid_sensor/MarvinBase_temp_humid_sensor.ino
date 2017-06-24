/*
MarvinBase

Basic controls of IoT Academy Marvin LoRa Development board.

This version supports:
	- Sending LoRa uplink messages using ABP 
	- Blink three times when sending data
  - Power control to RN2483 module

Instructions:
	- Get the latest version of the Arduino software
	- In Arduino IDE select Arduino Leonardo and com port of your device
	- Please adjust ABP adresses and key below to match yours
	- The loop() is where the actual stuff happens. Adjust input of send_lora_data() in void loop() to send your own data.
*/
// Port to assign the type of lora data (any port can be used between 1 and 223)
int     set_port  = 1;

// Some standard ports that depend on the layout of the Marvin
long    defaultBaudRate = 57600;
int     reset_port = 5;
int     RN2483_power_port = 6; //Note that an earlier version of the Marvin doesn't support seperate power to RN2483
int     led_port = 13;

//*** Set parameters here BEGIN ---->
String  set_nwkskey = "00000000000000000000000000000000";
String  set_appskey = "00000000000000000000000000000000";
String  set_devaddr = "00000000";
//*** <---- END Set parameters here

//** Set thigs right for the Grove temperature / humidity sensor
#include "DHT.h"      //download it here: https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
                      // press clone/download and then download as .zip
                      
#define DHTPIN A3     // A3 is closes to the usb port of Marvin

// define the type of sensor used (there are others)
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

/*
 * Setup() function is called when board is started. Marvin uses a serial connection to talk to your pc and a serial
 * connection to talk to the RN2483, these are both initialized in seperate functions. Also some Arduino port are 
 * initialized and a LED is called to blink when everything is done. 
 */
void setup() {

  Serial.begin(defaultBaudRate);
  Serial1.begin(defaultBaudRate);
  InitializeSerials(defaultBaudRate);
  initializeRN2483(RN2483_power_port, reset_port);
  pinMode(led_port, OUTPUT); // Initialize LED port  
  dht.begin();
  blinky();
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
    }
  int temp = (int) t;
  int hum = (int) h;  
  int tempdec = t * 100;
  int humdec = h * 100;

// Uncomment the line below for what you want to send

  send_LoRa_data(set_port, String(temp) + "F" + String(hum));      //send temp / hum as rounded int over lora
  //send_LoRa_data(set_port, String(tempdec) + "F" + String(humdec)); //send temp / hum as 4 digit integer (decimals included)

  blinky();
  delay(1000);
  read_data_from_LoRa_Mod();
  delay(30000);
}

void InitializeSerials(int baudrate)
{
  delay(1000);
  print_to_console("Serial ports initialised");
}

void initializeRN2483(int pwr_port, int rst_port)
{
  //Enable power to the RN2483
  pinMode(pwr_port, OUTPUT);
  digitalWrite(pwr_port, HIGH);
  print_to_console("RN2483 Powered up");
  delay(1000);
  
  //Disable reset pin
  pinMode(rst_port, OUTPUT);
  digitalWrite(rst_port, HIGH);

  //Configure LoRa module
  send_LoRa_Command("sys reset");
  read_data_from_LoRa_Mod();

  send_LoRa_Command("radio set crc off");
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set nwkskey " + set_nwkskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set appskey " + set_appskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set devaddr " + set_devaddr);
  read_data_from_LoRa_Mod();

  //For this commands some extra delay is needed.
  send_LoRa_Command("mac set adr on");
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac save");
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac join abp");
  delay(1000);
  read_data_from_LoRa_Mod();

}

void print_to_console(String message)
{
  Serial.println(message);
}

void read_data_from_LoRa_Mod()
{
  if (Serial1.available()) {
    String inByte = Serial1.readString();
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
}


void blinky()
{
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                     // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                     // wait for a second
  digitalWrite(led_port, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                     // wait for a second
  digitalWrite(led_port, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                     // wait for a second

}
