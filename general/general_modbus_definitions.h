// Functions that can be used by both Modbus Clients and Servers
// Not all functions have to be used by both

#ifndef FOKKER_MODBUS_GENENERAL_DEFINITIONS_H_
#define FOKKER_MODBUS_GENENERAL_DEFINITIONS_H_

#include "etl/optional.h"
#include "etl/array_view.h"
#include "etl/delegate.h"

class ModbusDefinitions
{
    constexpr uint8_t MSB(uint16_t value)
    {
        return value & 0xFF00;
    }
    constexpr uint8_t LSB(uint16_t value)
    {
        return value & 0x00FF;
    } 

    enum class FunctionCodes
    {
        READ_COILS = 0x01,
        READ_DISCRETE_INPUTS = 0x02,
        READ_HOLDING_REGISTERS = 0x03,
        READ_INPUT_REGISTERS = 0x04,
        WRITE_SINGLE_COIL = 0x05,
        WRITE_SINGLE_REGISTER = 0x06,
        DIAGNOSTICS = 0x08,
        GET_COMM_EVENT_COUNTER = 0x0B,
        WRITE_MULTIPLE_COILS = 0x0F,
        WRITE_MULTIPLE_REGISTERS = 0x10,
        REPORT_SERVER_ID = 0x11,
        MASK_WRITE_REGISTER = 0x16,
        READ_WRITE_MULTIPLE_REGISTERS = 0x17,
        READ_DEVICE_IDENTIFICATION = 0x0E,
        READ_DEVICE_IDENTIFICATION_2 = 0x2B,
    };
};

#endif // FOKKER_MODBUS_GENENERAL_DEFINITIONS_H_