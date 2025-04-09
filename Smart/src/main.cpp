#include <Arduino.h>
// #include <wdt.h>
#include <roof.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <canModule.h>
#include <esp_task_wdt.h>
// #include <www.h>
#define WDT_TIMEOUT 3
#define SERIAL 0

long baudrate = 100E3;

uint8_t CAN_TX_PIN = 5;
uint8_t CAN_RX_PIN = 4;
uint8_t CS_PIN = 8;
CanModule * CAN0, * CAN1;
packet_t packet;
void setup() {
  esp_task_wdt_init(WDT_TIMEOUT, true); 
  esp_task_wdt_add(NULL);
  CAN0 = new CanModule(&baudrate, &CAN_RX_PIN, &CAN_TX_PIN);
  CAN1 = new CanModule(&baudrate, &CS_PIN);
  #if SERIAL==1
    Serial.begin(250000);
    while (!Serial) {
    ; // wait for serial port to connect.
    }
  #endif 
}

void loop() {
  esp_task_wdt_reset();
  packet = CAN0->receive();
  if(packet.id == 0x381){

  }
  else if(packet.id!=0xFF){
    CAN1->sendPacketToCan(&packet);
  }
  packet = CAN1->receive();
  if(packet.id!=0xFF){
    CAN0->sendPacketToCan(&packet);
  }
  
  
  // ESP.wdtFeed(); // reset watchdog
  chceckRoofVars();
  // dnsServer.processNextRequest();
}

// void initEEPROM(){
//   size_t eepromSize = sizeof(NetworkParameters);
//   eepromSize+=sizeof(CANBUSParameters);
//   EEPROM.begin(eepromSize);
//   readFromEEPROM(paramNetwork);
//   readFromEEPROM(paramCAN);

// }
// void readFromEEPROM(NetworkParameters &data) {
//     EEPROM.get(0, data);
// }
// void readFromEEPROM(CANBUSParameters &data) {
//     EEPROM.get(sizeof(NetworkParameters), data);
// }

// void writeToEEPROM(const NetworkParameters &data) {
//     EEPROM.put(0, data);
// }
// void writeToEEPROM(const CANBUSParameters &data) {
//     EEPROM.put(sizeof(NetworkParameters), data);
// }