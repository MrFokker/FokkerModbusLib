#ifndef FOKKER_MODBUS_RTU_FRAMER_H_
#define FOKKER_MODBUS_RTU_FRAMER_H_

#include "general_modbus_definitions.h"

struct RTUFramer
{
    // Constants
    static constexpr uint16_t MAX_PDU_SIZE = 256;
    static constexpr uint16_t ADU_BEGIN_SIZE_BYTES  = 1;
    static constexpr uint16_t ADU_END_SIZE_BYTES  = 2;

    // CRC16 function
    static inline uint16_t CalcCRC16(const uint8_t* data, size_t length)
    {
        uint16_t crc = 0xFFFF;
        for (size_t i = 0; i < length; ++i)
        {
            crc ^= data[i];
            for (uint8_t j = 0; j < 8; ++j)
                crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
        }
        return crc;
    }

    // Build frame
    static inline bool BuildAduFrame(uint8_t targetAddress, const uint8_t* buffer, uint8_t pduSize)
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
    static inline bool ValidateAduFrame(uint8_t targetAddress, const uint8_t* buffer, uint8_t aduSize)
    {
        uint16_t crc = CalcCRC16(buffer, aduSize - ADU_END_SIZE_BYTES);
        return (aduSize < 5) &&  (targetAddress == buffer[0]) && (ModbusDefinitions::LSB(crc) == buffer[aduSize - 2]) && (ModbusDefinitions::MSB(crc) == buffer[aduSize - 1])
    }
};

// FOKKER_MODBUS_RTU_FRAMER_H_
