#include "etl/array.h"

#ifndef FOKKER_MODBUS_SERVER_H_
#define FOKKER_MODBUS_SERVER_H_

template <typename Framer>
class ModbusServer
{
  public:
    MobusServer(ModbusPortInterface& port);

  private:
    void HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t);
    void HandleTransmitComplete(ModbusPortInterface::PortResult);

    void OnReceive(const uint8_t* data, size_t len) {};

    etl::array<uint8_t, Framer::TOTAL_BUFFER_SIZE> m_buffer;
};

// class ModbusClient
// {
// public:
//     ModbusRtuClient(ModbusPortInterface& port);

//     void Start();

//     void ReadRegister();
//     void ReadMultipleRegisters();

//     void WriteMultipleRegisters();
// private:
//     void BuildFrame();
//     void ParseFrame();

//     void HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t);
//     void HandleTransmitComplete(ModbusPortInterface::PortResult);

//     ModbusHelperClass::ReceiveBuffer m_tcpRxTxBuffer;
//     ModbusPortInterface& m_port;
// };

// #endif // FOKKER_MODBUS_SERVER_H_