#include <Arduino.h>
// #include <CAN.h>
#include <MCP2515.h>
#include <ESP32SJA1000.h>

#define RXBUF_LEN 100

typedef struct {
  long id;
  byte rtr;
  byte ide;
  byte dlc;
  byte dataArray[20];
} packet_t;

class CanModule
{
private:
    const char SEPARATOR = ',';
    const char TERMINATOR = '\n';
    // const char RXBUF_LEN = 100;

    char * strToHex(char * str, byte * hexArray, byte * len);
    void rxParse(char * buf, int len);
    char getNum(char c);
    void printHex(long num);
    void printPacket(packet_t * packet);
public:
    void RXcallback();
    packet_t receive();
    void sendPacketToCan(packet_t * packet);
    CanModule(long * BUS_BAUDRATE, uint8_t * cs_pin);
    CanModule(long * BUS_BAUDRATE, uint8_t * RX_CAN, uint8_t * TX_CAN);
    ~CanModule();
};