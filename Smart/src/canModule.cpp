#include <CAN.h>
#include <canModule.h>


CanModule::CanModule(long * BUS_BAUDRATE, uint8_t * cs_pin)
{
  if (!CAN.begin(*BUS_BAUDRATE)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

CanModule::CanModule(long * BUS_BAUDRATE, uint8_t * RX_CAN, uint8_t * TX_CAN)
{
  CAN.setPins(*RX_CAN, *TX_CAN);
  if (!CAN.begin(*BUS_BAUDRATE)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

CanModule::~CanModule()
{
}

char CanModule::getNum(char c) {
  if (c >= '0' && c <= '9') { return c - '0'; }
  if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
  if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
  return 0;
}

char * CanModule::strToHex(char * str, byte * hexArray, byte * len) {
  byte *ptr = hexArray;
  char * idx;
  for (idx = str ; *idx != SEPARATOR && *idx != TERMINATOR; ++idx, ++ptr ) {
    *ptr = (getNum( *idx++ ) << 4) + getNum( *idx );
  }
  *len = ptr - hexArray;
  return idx;
}

void CanModule::rxParse(char * buf, int len) {
  packet_t rxPacket;
  char * ptr = buf;
  // All elements have to have leading zero!

  // ID
  byte idTempArray[8], tempLen;
  ptr = strToHex(ptr, idTempArray, &tempLen);
  rxPacket.id = 0;
  for (int i = 0; i < tempLen; i++) {
    rxPacket.id |= (long)idTempArray[i] << ((tempLen - i - 1) * 8);
  }

  // RTR
  ptr = strToHex(ptr + 1, &rxPacket.rtr, &tempLen);

  // IDE
  ptr = strToHex(ptr + 1, &rxPacket.ide, &tempLen);

  // DATA
  ptr = strToHex(ptr + 1, rxPacket.dataArray, &rxPacket.dlc);

  sendPacketToCan(&rxPacket);
}

//send
void CanModule::RXcallback() {
  static int rxPtr = 0;
  static char rxBuf[RXBUF_LEN];

  while (Serial.available() > 0) {
    if (rxPtr >= RXBUF_LEN) {
      rxPtr = 0;
    }
    char c = Serial.read();
    rxBuf[rxPtr++] = c;
    if (c == TERMINATOR) {
      rxParse(rxBuf, rxPtr);
      rxPtr = 0;
    }
  }
}

void CanModule::printHex(long num) {
  if ( num < 0x10 ){ Serial.print("0"); }
  Serial.print(num, HEX);
}

void CanModule::printPacket(packet_t * packet) {
  // packet format (hex string): [ID],[RTR],[IDE],[DATABYTES 0..8B]\n
  // example: 014A,00,00,1A002B003C004D\n
  printHex(packet->id);
  Serial.print(SEPARATOR);
  printHex(packet->rtr);
  Serial.print(SEPARATOR);
  printHex(packet->ide);
  Serial.print(SEPARATOR);
  // DLC is determinded by number of data bytes, format: [00]
  for (int i = 0; i < packet->dlc; i++) {
    printHex(packet->dataArray[i]);
  }
  Serial.print(TERMINATOR);
}


packet_t CanModule::receive() {
  // received a CAN packet
  packet_t rxPacket;
  rxPacket.id = CAN.packetId();
  rxPacket.rtr = CAN.packetRtr() ? 1 : 0;
  rxPacket.ide = CAN.packetExtended() ? 1 : 0;
  rxPacket.dlc = CAN.packetDlc();
  byte i = 0;
  while (CAN.available()) {
    rxPacket.dataArray[i++] = CAN.read();
    if (i >= (sizeof(rxPacket.dataArray) / (sizeof(rxPacket.dataArray[0])))) {
      break;
    }
  }
  printPacket(&rxPacket);
  return rxPacket;
}

void CanModule::sendPacketToCan(packet_t * packet) {
  for (int retries = 10; retries > 0; retries--) {
    bool rtr = packet->rtr ? true : false;
    if (packet->ide){
      CAN.beginExtendedPacket(packet->id, packet->dlc, rtr);
    } else {
      CAN.beginPacket(packet->id, packet->dlc, rtr);
    }
    CAN.write(packet->dataArray, packet->dlc);
    if (CAN.endPacket()) {
      // success
      break;
    } else if (retries <= 1) {
      return;
    }
  }
}