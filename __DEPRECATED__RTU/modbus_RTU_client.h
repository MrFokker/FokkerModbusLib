#include "etl/array.h"
#include "general_modbus_functions.h"
#include "modbus_port_interface.h"
#include "modbus_RTU_framer.h

#ifndef FOKKER_MODBUS_RTU_CLIENT
#define FOKKER_MODBUS_RTU_CLIENT

class ModbusRtuClient : public ModbusHelperClass
{
public:
    ModbusRtuClient(ModbusPortInterface& port);

    void Start();

    void ReadRegister();
    void ReadMultipleRegisters();

    void WriteMultipleRegisters();
private:
    void BuildFrame();
    void ParseFrame();

    void HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t);
    void HandleTransmitComplete(ModbusPortInterface::PortResult);

    
    ModbusHelperClass::ReceiveBuffer m_tcpRxTxBuffer;
    ModbusPortInterface& m_port;
};

#endif