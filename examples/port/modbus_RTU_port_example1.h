#include "modbus_RTU_port.h"

#ifndef FOKKER_MODBUS_RTU_PORT
#define FOKKER_MODBUS_RTU_PORT

class ModbusPort : public ModbusPortInterface
{
public:
    ModbusPort() = default;

    bool Receive(etl::array_view<BufferDataType> transmitBuffer, uint16_t timeoutMs, const ReceiveCallback &receiveCallback) override;
    bool Transmit(etl::array_view<BufferDataType> transmitBuffer, const TransmitCallback &transmitCallback) override;

}
#endif // FOKKER_MODBUS_RTU_PORT
