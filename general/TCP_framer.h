#ifndef FOKKER_MODBUS_TCP_FRAMER_H_
#define FOKKER_MODBUS_TCP_FRAMER_H_

#include "general_modbus_definitions.h"

struct TCPFramer
{
    // Constants
    static constexpr uint16_t MAX_PDU_SIZE = 260;
    static constexpr uint16_t ADU_BEGIN_SIZE_BYTES  = 7;
    static constexpr uint16_t ADU_END_SIZE_BYTES  = 0;

    // Build frame
    static inline bool BuildAduFrame(const uint8_t targetAddress, uint8_t* buffer, const uint8_t pduSize)
    {
        if ((pduSize + ADU_BEGIN_SIZE_BYTES + ADU_END_SIZE_BYTES) > MAX_PDU_SIZE) 
        {
            return false;
        }
        buffer[0] = targetAddress;
        uint16_t crc = CalcCRC16(buffer, ADU_BEGIN_SIZE_BYTES + pduSize);
        buffer[ADU_BEGIN_SIZE_BYTES + pduSize] = ModbusDefinitions::LSB(crc);
        buffer[ADU_BEGIN_SIZE_BYTES + pduSize + 1] = ModbusDefinitions::MSB(crc);
        return true;
    }

    // Parse frame
    static inline bool ValidateAduFrame(const uint8_t targetAddress, const uint8_t* buffer, const uint8_t aduSize)
    {
        uint16_t crc = CalcCRC16(buffer, aduSize - ADU_END_SIZE_BYTES);
        return (aduSize < 5) &&  (targetAddress == buffer[0]) && (ModbusDefinitions::LSB(crc) == buffer[aduSize - 2]) && (ModbusDefinitions::MSB(crc) == buffer[aduSize - 1]);
    }
};

#endif // FOKKER_MODBUS_TCP_FRAMER_H_
