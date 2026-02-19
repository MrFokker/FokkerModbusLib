
// MockTransport.h
#ifndef FOKKER_MODBUS_RTU_PORT_MOCK_H_
#define FOKKER_MODBUS_RTU_PORT_MOCK_H_

#include <gmock/gmock.h>
#include "modbus_port_interface.h"

class ModbusRtuPortMock : public ModbusPortInterface
{
public:
    MOCK_METHOD(void, Start, (), (override));

    MOCK_METHOD(bool, Receive, (etl::array_view<uint8_t>, uint16_t timeoutMs, const ModbusPortInterface::ReceiveCallback &), (override));
    MOCK_METHOD(bool, Transmit, (etl::array_view<uint8_t>, const ModbusPortInterface::TransmitCallback &), (override));
};

#endif // FOKKER_MODBUS_RTU_PORT_MOCK_H_
