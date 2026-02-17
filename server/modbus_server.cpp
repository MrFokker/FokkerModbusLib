#include "modbus_RTU_client.h"

ModbusServer::ModbusServer(ModbusPortInterface &port)
    : m_port(port), ModbusHelperClass(m_tcpRxTxBuffer, m_tcpRxTxBuffer)
{
}

void ModbusRtuClient::Start()
{
    m_port.Start();
}


void ModbusRtuClient::BuildResponseFrame()
{
}
void ModbusRtuClient::ParseFrame()
{
}

void ModbusServer::HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t length)
{
    if (!Framer::ValidateRequest(m_buffer, length))
    {
    }

    // Parse PDU (shared)
    HandlePDU(...);

    // Wrap response
    Framer::WrapResponse();
    m_port.Transmit(m_buffer.data(), m_buffer.size(), {});
    
}

void ModbusServer::HandleTransmitComplete(ModbusPortInterface::PortResult)
{
    ParseFrame();
}