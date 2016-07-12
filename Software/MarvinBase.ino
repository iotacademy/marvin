/*
MarvinBase

Basic controls of IoT Academy Marvin LoRa Development board.

This version supports:
	- Sending LoRa uplink messages using ABP 
	- Blink three times when sending data

Instructions:
	- In Arduino IDE select Arduino Leonardo and com port of your device
	- Please adjust ABP-key parameters below to match yours
	- Adjust input of send_lora_data() in void loop() to send your own data
*/

int     defaultBaudRate = 57600;
int     set_port  = 1;
int     reset_port = 5;

//*** Set parameters here BEGIN ---->
String  set_nwkskey = "00000000000000000000000000000000";
String  set_appskey = "00000000000000000000000000000000";
String  set_devaddr = "00000000";
//*** <---- END Set parameters here



void setup() {
  InitializeSerials(defaultBaudRate);
  initializeRN2483(reset_port);
  pinMode(13, OUTPUT);
  blinky();
}

void loop() {

  send_LoRa_data(set_port, "1337");
  blinky();
  delay(1000);
  read_data_from_LoRa_Mod();
  delay(30000);
}

void InitializeSerials(int baudrate)
{
  Serial.begin(57600);
  Serial1.begin(57600);
  print_to_console("Serial ports initialised");
}

void initializeRN2483(int rst_port)
{
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
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac save");
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac join abp");
  delay(1000);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("radio set crc off");
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
  Serial.println("Now sending: " + cmd);
  Serial1.println(cmd);
  delay(500);
}

void send_LoRa_data(int tx_port, String rawdata)
{
  send_LoRa_Command("mac tx uncnf " + String(tx_port) + String(" ") + rawdata);
}


void blinky()
{
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second

}


