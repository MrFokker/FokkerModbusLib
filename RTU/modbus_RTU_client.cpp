#include "modbus_RTU_client.h"

ModbusRtuClient::ModbusRtuClient(ModbusPortInterface &port)
    : m_port(port), ModbusHelperClass(m_tcpRxTxBuffer, m_tcpRxTxBuffer)
{
}

void ModbusRtuClient::Start()
{
}
void ModbusRtuClient::ReadRegister()
{
    BuildFrame();
}
void ModbusRtuClient::ReadMultipleRegisters()
{
    BuildFrame();
}
void ModbusRtuClient::WriteMultipleRegisters()
{
    BuildFrame();
}
void ModbusRtuClient::BuildFrame()
{
}
void ModbusRtuClient::ParseFrame()
{
}
void ModbusRtuClient::HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t)
{
    ParseFrame();
}
void ModbusRtuClient::HandleTransmitComplete(ModbusPortInterface::PortResult)
{
    ParseFrame();
}