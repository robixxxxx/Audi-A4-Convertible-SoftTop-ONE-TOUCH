#include <map>
#include <string.h>
typedef struct data_point
{
    std::string name;
    uint8_t data_size;
    uint8_t byte_position;
    uint8_t bit_position;
    uint8_t length;
};


typedef struct config
{
    long CAN_Baudrate;
    //map<nodeID, map<?,datapoint>
    std::map<uint32_t, std::map<uint8_t, data_point>> data;

};

class Config
{
private:
    
public:
    Config(/* args */);
    ~Config();
};

Config::Config(/* args */)
{
}

Config::~Config()
{
}
