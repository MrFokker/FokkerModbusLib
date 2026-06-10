#include "modbus_RTU_port_example1.h"


void ModbusPortExample::Start()
{
    // Stop Peripheral and empty buffers
    (void);
}

void ModbusPortExample::Cleanup()
{
    // Stop Peripheral and empty buffers
    (void);
}

bool ModbusPortExample::Receive(etl::array_view<uint8_t> transmitBuffer, uint16_t timeoutMs, const ModbusPortInterface::ReceiveCallback &receiveCallback)
{
    (void)transmitBuffer;
    (void)timeoutMs;
    (void)receiveCallback;
    // Start sending message
    return false;
}
bool ModbusPortExample::Transmit(etl::array_view<uint8_t> transmitBuffer, const ModbusPortInterface::TransmitCallback &transmitCallback)
{
    (void)transmitBuffer;
    (void)transmitCallback;
    // Start receiving
    return false;
}
