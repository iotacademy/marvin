/**
 * @brief      Implementation of the Marvin Base file put into a class.
 * @author     Jimmy van den Berg
 * @date       3 August 2017
 */

#ifndef _MARVINLIB_H_
#define _MARVINLIB_H_

#include "Arduino.h"

namespace marvin{
	//*** Set parameters here BEGIN ---->
	static const String set_nwkskey = "00000000000000000000000000000000";
	static const String set_appskey = "00000000000000000000000000000000";
	static const String set_devaddr = "00000000";
	//*** <---- END Set parameters here

	class LoRaCom{
	public:
		/**
		 * @brief      Setup LoRa module like installing the keys.
		 *
		 * @return     Zero on success, -1 otherwise.
		 */
		static int8_t initializeLoRa(){
			if(exec_general_LoRa_command("radio set crc off") != 0){
				return -1;
			}
			if(mac_set("nwkskey " + set_nwkskey) != 0){
				return -1;
			}
			if(mac_set("appskey " + set_appskey) != 0){
				return -1;
			}
			if(mac_set("devaddr " + set_devaddr) != 0){
				return -1;
			}
			if(mac_set("adr on") != 0){
				return -1;
			}
			if(exec_general_LoRa_command("mac save") != 0){
				return -1;
			}
			if(exec_general_LoRa_command("mac join abp") != 0){
				return -1;
			}
			return 0;
		}
		/**
		 * @brief      Sends data over LoRa.
		 *
		 * @param[in]  tx_port  The transmit port
		 * @param[in]  data     The data
		 *
		 * @return     Zero on success, -1 otherwise.
		 */
		static int8_t sendData(uint8_t tx_port, String data){
			return exec_general_LoRa_command("mac tx uncnf " + String(tx_port) + String(" ") + data);
		}
		/**
		 * @brief      Execute general LoRa command.
		 *
		 *             This simply puts the string to the LoRa module and checks
		 *             if the module returns oké.
		 *
		 * @param[in]  command  The command as string
		 *
		 * @return     Zero on success, -1 otherwise.
		 */
		static int8_t exec_general_LoRa_command(String command){
			notify_user_LoRa_send(command);
			Serial1.println(command);
			// Set timeout to 3 seconds.
			constexpr uint16_t timeoutMs = 3000;
			uint16_t sendAtMillis = millis();
			while((millis() & 0xFF) - sendAtMillis > timeoutMs && Serial1.available() == 0);
			// Check if we actually received something. If not, timeout error.
			if(Serial1.available() == 0){
				return -1;
			}
			else{
				// Received message on time. Check if ok.
				String response = Serial1.readStringUntil('\n');
				notify_user_LoRa_response(response);
				if(response == "ok\r"){
					return 0;
				}
				else{
					return -1;
				}
			}
		}
		/**
		 * @brief      Send a mac set command.
		 *
		 *             Provides return asap instead of using a delay.
		 *
		 * @param[in]  command  The parameter and its value.
		 *
		 * @return     Zero on success. -1 on error.
		 */
		static int8_t mac_set(String command){
			return exec_general_LoRa_command("mac set " + command);
		}
	private:
		/**
		 * @brief      Print (string) data to console.
		 *
		 *             Generalize the way to print to console. Could be substituted
		 *             in the future for something else.
		 *
		 * @param[in]  data  The (string) data to be printed.
		 */
		static void print_to_console(String data){
			Serial.println(data);
		}
		static void nofity_user(String what){
			print_to_console("Note: " + what);
		}
		static void notify_user_LoRa_response(String data){
			print_to_console("From LoRa: " + data);
		}
		static void notify_user_LoRa_send(String data){
			print_to_console("To LoRa: " + data);
		}
	};
}

#endif
