#include "modbus_port_interface.h"

#ifndef FOKKER_MODBUS_RTU_PORT
#define FOKKER_MODBUS_RTU_PORT

class ModbusPortExample : public ModbusPortInterface
{
  public:
    ModbusPort() = default;

    void Start() override;
    void Cleanup() override;

    bool Receive(etl::array_view<uint8_t>                    transmitBuffer,
                 uint16_t                                    timeoutMs,
                 const ModbusPortInterface::ReceiveCallback& receiveCallback) override;
    bool Transmit(etl::array_view<uint8_t> transmitBuffer, const ModbusPortInterface::TransmitCallback& transmitCallback) override;
}
#endif // FOKKER_MODBUS_RTU_PORT
