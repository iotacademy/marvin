/* Marvin Piezo Vibration Sensor examples - bd
   This basically sends a LoRA packet whenever the piezo sensor gets hit.
   There's no sleep yet, nothing fancy, but it works.
*/
#include "CayenneLPP.h"
#include <MarvinLib.h>

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

// Some global items
String reader = "";

// counter
int   count = 1;

marvin::LoRaCom loraModule;

void setup() {
  InitializeSerials(defaultBaudRate);
  loraModule.initializeLoRa();
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

    loraModule.sendData(set_port, payload);
    delay(10000);
    Serial.println(count);
    count = count + 1;

  //} // end if

} // end loop()


void InitializeSerials(long baudrate) {
  Serial.begin(baudrate);
  Serial1.begin(baudrate);
  delay(1000);
  Serial.println("Serial ports initialised");
}
