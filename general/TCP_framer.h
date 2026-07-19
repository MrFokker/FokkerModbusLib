#ifndef FOKKER_MODBUS_TCP_FRAMER_H_
#define FOKKER_MODBUS_TCP_FRAMER_H_

#include "general_modbus_definitions.h"

struct TCPFramer {
    // Constants
    static constexpr uint16_t ADU_BEGIN_SIZE_BYTES = 7;
    static constexpr uint16_t ADU_END_SIZE_BYTES   = 0;
    static constexpr uint16_t MINIMUM_PDU_SIZE     = ADU_BEGIN_SIZE_BYTES + ADU_END_SIZE_BYTES + ModbusDefinitions::MINIMUM_ADU_SIZE;
    static constexpr uint16_t MAX_PDU_SIZE         = 260;
    static constexpr uint16_t MODBUS_PROTOCOL_ID   = 0;

    // Build frame
    static inline bool BuildAduFrame(uint8_t* buffer, const uint8_t pduSize, const uint16_t transactionId, const uint8_t unitId)
    {
        if ((pduSize + ADU_BEGIN_SIZE_BYTES + ADU_END_SIZE_BYTES) > MAX_PDU_SIZE)
            return false;

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
        if ((aduSize < MINIMUM_PDU_SIZE) || (aduSize > MAX_PDU_SIZE))
            return false;

        const uint16_t expectedLength = aduSize - 6;
        bool           valid          = true;

        valid = valid && (buffer[0] == ModbusDefinitions::MSB(transactionId));
        valid = valid && (buffer[1] == ModbusDefinitions::LSB(transactionId));
        valid = valid && (buffer[2] == ModbusDefinitions::MSB(MODBUS_PROTOCOL_ID));
        valid = valid && (buffer[3] == ModbusDefinitions::LSB(MODBUS_PROTOCOL_ID));
        valid = valid && (buffer[4] == ModbusDefinitions::MSB(expectedLength));
        valid = valid && (buffer[5] == ModbusDefinitions::LSB(expectedLength));
        valid = valid && (buffer[6] == unitId);

        return valid;
    }
};

#endif // FOKKER_MODBUS_TCP_FRAMER_H_
