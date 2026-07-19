#ifndef FOKKER_MODBUS_RTU_FRAMER_H_
#define FOKKER_MODBUS_RTU_FRAMER_H_

#include "general_modbus_definitions.h"

struct RTUFramer {
    // Constants
    static constexpr uint16_t ADU_BEGIN_SIZE_BYTES = 1;
    static constexpr uint16_t ADU_END_SIZE_BYTES   = 2;
    static constexpr uint16_t MINIMUM_PDU_SIZE     = ADU_BEGIN_SIZE_BYTES + ADU_END_SIZE_BYTES + ModbusDefinitions::MINIMUM_ADU_SIZE;
    static constexpr uint16_t MAX_PDU_SIZE         = 256;

    // CRC16 function
    static inline uint16_t CalcCRC16(const uint8_t* data, size_t length)
    {
        uint16_t crc = 0xFFFF;
        for (size_t i = 0; i < length; ++i) {
            crc ^= data[i];
            for (uint8_t j = 0; j < 8; ++j)
                crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
        }
        return crc;
    }

    // Build frame
    static inline bool BuildAduFrame(uint8_t* buffer, const uint8_t pduSize, const uint16_t, const uint8_t targetAddress)
    {
        if ((pduSize + ADU_BEGIN_SIZE_BYTES + ADU_END_SIZE_BYTES) > MAX_PDU_SIZE)
            return false;

        buffer[0]                                  = targetAddress;
        const uint16_t crc                         = CalcCRC16(buffer, ADU_BEGIN_SIZE_BYTES + pduSize);
        buffer[ADU_BEGIN_SIZE_BYTES + pduSize]     = ModbusDefinitions::LSB(crc);
        buffer[ADU_BEGIN_SIZE_BYTES + pduSize + 1] = ModbusDefinitions::MSB(crc);
        return true;
    }

    // Parse frame
    static inline bool ValidateAduFrame(const uint8_t* buffer, const uint8_t aduSize, const uint16_t, const uint8_t targetAddress)
    {
        if ((aduSize < MINIMUM_PDU_SIZE) || (aduSize > MAX_PDU_SIZE))
            return false;

        const uint16_t calculatedCrc = CalcCRC16(buffer, aduSize - ADU_END_SIZE_BYTES);
        bool           valid         = true;

        valid = valid && (targetAddress == buffer[0]);
        valid = valid && (ModbusDefinitions::LSB(calculatedCrc) == buffer[aduSize - 2]);
        valid = valid && (ModbusDefinitions::MSB(calculatedCrc) == buffer[aduSize - 1]);

        return valid;
    }
};

#endif // FOKKER_MODBUS_RTU_FRAMER_H_
