
// MockTransport.h
#ifndef FOKKER_MODBUS_RTU_PORT_MOCK_H_
#define FOKKER_MODBUS_RTU_PORT_MOCK_H_

#include <gmock/gmock.h>
#include "modbus_port_interface.h"

class ModbusRtuPortMock : public ModbusPortInterface
{
public:
    MOCK_MEHTOD(bool, Receive, (etl::array_view<BufferDataType>, uint16_t timeoutMs, const ReceiveCallback &), (override));
    MOCK_MEHTOD(bool, Transmit, (etl::array_view<BufferDataType>, const TransmitCallback &), (override));
};

#endif // FOKKER_MODBUS_RTU_PORT_MOCK_H_
