// Functions that can be used by both Modbus Clients and Servers
// Not all functions have to be used by both

#include "etl/byte_stream.h"
#include "etl/optional.h"
#include "etl/array_view.h"
#include "etl/delegate.h"

#ifndef FOKKER_MODBUS_RTU_PORT_INTERFACE
#define FOKKER_MODBUS_RTU_PORT_INTERFACE

class ModbusPortInterface
{
public:
    enum class PortResult 
    {
        TIMEOUT_ERROR,
        BUSY_ERROR,
        OVERLOAD_ERROR,
        NO_ERROR,
    };

    ModbusPortInterface() = default;
    virtual ~ModbusPortInterface() = default;

    using ReceiveCallback = etl::delegate<void(PortResult, uint16_t)>; // count
    using TransmitCallback = etl::delegate<void(PortResult)>;

    virtual void Start() {};
    virtual void Cleanup() {};

    virtual bool Receive(etl::array_view<uint8_t> transmitBuffer, uint16_t timeoutMs, const ReceiveCallback &receiveCallback) = 0;
    virtual bool Transmit(etl::array_view<uint8_t> transmitBuffer, const TransmitCallback &transmitCallback) = 0;
};

#endif // FOKKER_MODBUS_RTU_PORT_INTERFACE
