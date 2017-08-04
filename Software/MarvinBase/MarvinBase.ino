/**
 * @file MarvinBase.ino
 *
 * Basic controls of IoT Academy Marvin LoRa Development board.
 *
 * This version supports:
 * - Sending LoRa up-link messages using ABP
 * - Blink three times when sending data
 * - Power control to RN2483 module
 *
 * Instructions:
 * - Get the latest version of the Arduino software
 * - In Arduino IDE select Arduino Leonardo and com port of your device
 * - Please adjust ABP addresses and key below to match yours
 * - The loop() is where the actual stuff happens. Adjust input of
 *   send_lora_data() in void loop() to send your own data.
 */

#include <MarvinLib.h>

// Port to assign the type of lora data (any port can be used between 1 and 223)
uint8_t     set_port  = 1;

// Some standard ports that depend on the layout of the Marvin
static constexpr long    defaultBaudRate = 57600;
const uint8_t reset_port = 5;
const uint8_t RN2483_power_port = 6; // Note that an earlier version of the Marvin doesn't support
                                     // separate power to RN2483
const uint8_t led_port = 13;

//------------------------------------------------------------------------------
/// Some variables for the Grove temperature sensor v1.2 :
/// http://wiki.seeed.cc/Grove-Temperature_Sensor_V1.2/
#include <math.h>
const uint32_t R_fixed = 100000;
const uint16_t B_value = 4275; // B value of the thermistor
const uint8_t pinTempSensor = A3;     // Grove - Temperature Sensor connect to A3 - This is the port
                                      // closest to the USB port of Marvin

const float zeroKelvinToZeroCelcius = 273.15;
const float zeroKelvinToRoomCelcius = 298.15;

marvin::LoRaCom loraModule;

/** 
 * function is called when board is started.
 */
void setup() {
	Serial.begin(defaultBaudRate);
	Serial1.begin(defaultBaudRate);
	delay(5000);
	pinMode(led_port, OUTPUT); // Initialize LED port  
	while(loraModule.initializeLoRa() != 0){
		// Initializing failed. Try again after delay.
		for (int i = 0; i < 5; ++i){
			digitalWrite(led_port, HIGH);
			delay(100);
			digitalWrite(led_port, LOW);
			delay(100);
		}
	}
	blinky();
}

void loop() {
	float temperature = getTemperature();
	Serial.print("temperature = ");
	Serial.println(temperature);
	int temp = (int)temperature;  // We cast the float value to an integer because the format for the
	                              // RN2483 is Hexadecimal, meaning the . character is not included.
	                              // You can also multiply the temperature by 100 to send the decimals
	                              // in the payload
	loraModule.sendData(set_port, String(temp));
	blinky();
	delay(30000);
}

float getTemperature(){
	uint16_t a = analogRead(pinTempSensor);
	// Here floats are avoided for now.
	uint32_t R = long(1023 - a) * R_fixed / a;
	return float(zeroKelvinToRoomCelcius * B_value) / (B_value + (zeroKelvinToRoomCelcius * log(float(R) / R_fixed))) - zeroKelvinToZeroCelcius;   //convert to temperature via datasheet ;;
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
