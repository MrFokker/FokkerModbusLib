#ifndef FOKKER_MODBUS_TCP_FRAMER_H_
#define FOKKER_MODBUS_TCP_FRAMER_H_

#include "general_modbus_definitions.h"

struct TCPFramer
{
    // Constants
    static constexpr uint16_t ADU_BEGIN_SIZE_BYTES  = 7;
    static constexpr uint16_t ADU_END_SIZE_BYTES  = 0;
    static constexpr uint16_t MINIMUM_PDU_SIZE = ADU_BEGIN_SIZE_BYTES + ADU_END_SIZE_BYTES + ModbusDefinitions::MINIMUM_ADU_SIZE;
    static constexpr uint16_t MAX_PDU_SIZE = 260;
    static constexpr uint16_t MODBUS_PROTOCOL_ID = 0;

    // Build frame
    static inline bool BuildAduFrame(uint8_t* buffer, const uint8_t pduSize,  const uint16_t transactionId, const uint8_t unitId)
    {
        if ((pduSize + ADU_BEGIN_SIZE_BYTES + ADU_END_SIZE_BYTES) > MAX_PDU_SIZE) 
        {
            return false;
        }
        const uint16_t length = pduSize + sizeof(unitId);

        buffer[0] = ModbusDefinitions::MSB(transactionId);
        buffer[1] = ModbusDefinitions::LSB(transactionId);
        buffer[2] = ModbusDefinitions::MSB(MODBUS_PROTOCOL_ID);
        buffer[3] = ModbusDefinitions::LSB(MODBUS_PROTOCOL_ID);
        buffer[4] = ModbusDefinitions::MSB(length);
        buffer[5] = ModbusDefinitions::LSB(length);
        buffer[6] = unitId;
        return true;
    }

    // Parse frame
    static inline bool ValidateAduFrame(const uint8_t* buffer, const uint8_t aduSize, const uint16_t transactionId, const uint8_t unitId)
    {
        if ((aduSize <MINIMUM_PDU_SIZE) || (aduSize > MAX_PDU_SIZE))
        {
            return false;
        }
        const uint16_t expectedLength = aduSize - 6;

        bool success = true;
        success = success && (buffer[0] == ModbusDefinitions::MSB(transactionId));
        success = success && (buffer[1] == ModbusDefinitions::LSB(transactionId));
        success = success && (buffer[2] == ModbusDefinitions::MSB(MODBUS_PROTOCOL_ID));
        success = success && (buffer[3] == ModbusDefinitions::LSB(MODBUS_PROTOCOL_ID));
        success = success && (buffer[4] == ModbusDefinitions::MSB(expectedLength));
        success = success && (buffer[5] == ModbusDefinitions::LSB(expectedLength));
        success = success && (buffer[6] == unitId);  
        return success;
    }
};

#endif // FOKKER_MODBUS_TCP_FRAMER_H_
