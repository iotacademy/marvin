/* Marvin Piezo Vibration Sensor examples - bd
   This basically sends a LoRA packet whenever the piezo sensor gets hit.
   There's no sleep yet, nothing fancy, but it works.
*/
#include "CayenneLPP.h"

// set max size
int     MAX_SIZE = 52;
CayenneLPP lpp(MAX_SIZE);

// Port to assign the type of lora data (any port can be used between 1 and 223)
int     set_port  = 1;

// Some standard ports that depend on the layout of the Marvin
long    defaultBaudRate = 57600;
int     reset_port = 5;
int     RN2483_power_port = 6;
int     led_port = 13;

//*** Set parameters here BEGIN ---->
// NOTE: Devices joining the network via ABP, what we're doing for the hackathon,
// only require DevADDR, NWKSKey, and APPSKey. DevEUI will be assigned by Network Server.
String  set_devaddr = "00000000"; // Put your 8 hex char here
String  set_nwkskey = "01010101010101010101010101010101"; // Put your 32 hex char here
String  set_appskey = "01010101010101010101010101010101"; // Put your 32 hex char here
//*** <---- END Set parameters here

// Some global items
String reader = "";

// counter
int   count = 1;

void setup() {
  InitializeSerials(defaultBaudRate);
  initializeRN2483(RN2483_power_port, reset_port);
  Serial.println("STARTUP..." );

  pinMode(A2, INPUT_PULLUP);
  digitalWrite(A2, LOW);
  pinMode(A3, INPUT_PULLUP);
  digitalWrite(A3, LOW);
}


void loop() {
  //int buttonState = digitalRead(A3);
  //int button2State = digitalRead(A2);

  //if (buttonState == 1 || button2State == 1) {
    char payload[MAX_SIZE] = "50";

    lpp.reset();

    // Stub out payloads if you want 
    lpp.addDigitalInput(1, 1);
    // lpp.addTemperature(12, 22.5);
    // lpp.addBarometricPressure(2, 1073.21);
    // lpp.addGPS(3, 52.37365, 4.88650, 2);

//These values are hard coded. For actual Marvin sensor values 
//define a variable with a analog/digitalRead on the pin that the sensor is on.
//The call this valuable on the second place in the above lpp.add~ 

    uint8_t buff = *lpp.getBuffer();

    Serial.print("Buffer size:" );
    Serial.println(lpp.getSize());

    for (int i = 0; i < lpp.getSize(); i++) {
      char tmp[16];

      sprintf(tmp, "%.2X",(lpp.getBuffer())[i]);
      strcat(payload, tmp);
    }

    Serial.print("Buffer content:" );
    Serial.println(payload);

    send_LoRa_data(set_port, payload);
    delay(500);
    read_data_from_LoRa_Mod();
    delay(10000);
    Serial.println(count);
    count = count + 1;

  //} // end if

} // end loop()


void InitializeSerials(long baudrate) {
  Serial.begin(baudrate);
  Serial1.begin(baudrate);
  delay(1000);
  print_to_console("Serial ports initialised");
}


void initializeRN2483(int pwr_port, int rst_port) {
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

  send_LoRa_Command("mac set nwkskey " + set_nwkskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set appskey " + set_appskey);
  read_data_from_LoRa_Mod();

  send_LoRa_Command("mac set devaddr " + set_devaddr);
  read_data_from_LoRa_Mod();

  //For this commands some extra delay is needed.
  send_LoRa_Command("mac set adr on");
  //send_LoRa_Command("mac set dr 0"); //uncomment this line to fix the RN2483 on SF12 (dr=DataRate)
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

void print_to_console(String message) {
  Serial.println(message);
}

void read_data_from_LoRa_Mod() {
  if (Serial1.available()) {
    String inByte = Serial1.readString();
    Serial.println(inByte);
  }

}

void send_LoRa_Command(String cmd) {
  print_to_console("Now sending: " + cmd);
  Serial1.println(cmd);
  delay(500);
}

void send_LoRa_data(int tx_port, String rawdata) {
  send_LoRa_Command("mac tx uncnf " + String(tx_port) + String(" ") + rawdata);
  //send_LoRa_Command("mac tx cnf " + String(tx_port) + String(" ") + rawdata);
}

