// Functions that can be used by both Modbus Clients and Servers
// Not all functions have to be used by both

// Source (PDU spec): https://www.modbus.org/file/secure/modbusprotocolspecification.pdf

#ifndef FOKKER_MODBUS_GENENERAL_DEFINITIONS_H_
#define FOKKER_MODBUS_GENENERAL_DEFINITIONS_H_

#include <cstdint>

class ModbusDefinitions
{
  public:
    static constexpr uint8_t MINIMUM_ADU_SIZE = 2; // Function code +
    enum class FunctionCodes {
        DEFAULT_VALUE                    = 0x00,
        READ_COILS                       = 0x01,
        READ_DISCRETE_INPUTS             = 0x02,
        READ_HOLDING_REGISTERS           = 0x03,
        READ_INPUT_REGISTERS             = 0x04, // Remove S
        WRITE_SINGLE_COIL                = 0x05,
        WRITE_SINGLE_REGISTER            = 0x06,
        READ_EXCEPTION_STATUS            = 0x07,
        DIAGNOSTICS                      = 0x08, // Subcode: 00-18,20
        GET_COM_EVENT_COUNTER            = 0x0B,
        WRITE_MULTIPLE_COILS             = 0x0F,
        WRITE_MULTIPLE_REGISTERS         = 0x10,
        REPORT_SERVER_ID                 = 0x11,
        READ_FILE_RECORD                 = 0x14,
        WRITE_FILE_RECORD                = 0x15,
        MASK_WRITE_REGISTER              = 0x16,
        READ_WRITE_MULTIPLE_REGISTERS    = 0x17,
        READ_FIFO_QUEUE                  = 0x18,
        GET_COM_EVENT_LOG                = 0x0C,
        READ_DEVICE_IDENTIFICATION       = 0x2B, // Subcode: 14
        ENCAPSULATED_INTERFACE_TRANSPORT = 0x2B, // Subcode: 13, 14
        CAN_OPEN_GENERAL_REFERENCE       = 0x2B, // Subcode: 13
    };

    enum class ExceptionCodes {
        DEFAULT_VALUE                           = 0x00,
        ILLEGAL_FUNCTION                        = 0x01,
        ILLEGAL_DATA_ADDRESS                    = 0x02,
        ILLEGAL_DATA_VALUE                      = 0x03,
        SERVER_DEVICE_FAILURE                   = 0x04,
        ACKNOWLEDGE                             = 0x05,
        SERVER_DEVICE_BUSY                      = 0x06,
        MEMORY_PARITY_ERROR                     = 0x08,
        GATEWAY_PATH_UNAVAILABLE                = 0x0A,
        GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 0x0B,
    };

    // Function Code Dependend:
    enum class DiagnosticsSubFunctionCodes {
        RETURN_QUERY_DATA             = 0x0000,
        RESTART_COMMUNICATIONS_OPTION = 0x0001,
        RETURN_DIAGNOSTIC_REGISTER    = 0x0002,
        CHANGE_ASCII_INPUT_DELIMITER  = 0x0003, // Not supported
        FORCE_LISTEN_ONLY_MODE        = 0x0004,
        // 0x0005 – 0x0009 RESERVED
        CLEAR_COUNTERS_AND_DIAGNOSTIC_REGISTER = 0x000A,
        RETURN_BUS_MESSAGE_COUNT               = 0x000B,
        RETURN_BUS_COMM_ERROR_COUNT            = 0x000C,
        RETURN_BUS_EXCEPTION_ERROR_COUNT       = 0x000D,
        RETURN_SERVER_MESSAGE_COUNT            = 0x000E,
        RETURN_SERVER_NO_RESPONSE_COUNT        = 0x000F, // NAK: Negative Acknowledge
        RETURN_SERVER_NAK_COUNT                = 0x0010,
        RETURN_SERVER_BUSY_COUNT               = 0x0011,
        RETURN_BUS_CHAR_OVERRUN_COUNT          = 0x0012,
        // 0x0013 RESERVED
        CLEAR_OVERRUN_COUNTER_AND_FLAG = 0x0014,
        // 0x0015 - 0xFFFF RESERVED
    };

    struct FileRecordData {
        uint8_t  m_referenceType = 0x06;   // Allowed: 6
        uint16_t m_fileNumber    = 0x0000; // Allowed: 1 to 65.535
        uint16_t m_recordNumber  = 0xFFFF; // Allowed: 0 to 9999
        uint16_t m_recordLength  = 0x0000; // Allowed: 1 to 122 (* uint16_t)
    };

    static constexpr uint8_t LSB(const uint16_t value) noexcept
    {
        return static_cast<uint8_t>(value & 0xFF);
    };
    static constexpr uint8_t MSB(const uint16_t value) noexcept
    {
        return static_cast<uint8_t>(value >> 8);
    };
    static constexpr bool IsErrorResponse(const uint8_t functionCode) noexcept
    {
        return functionCode & 0x80;
    };
    static constexpr FunctionCodes ToFunctionCode(const uint8_t functionCode) noexcept
    {
        return static_cast<FunctionCodes>(functionCode & 0x7F);
    };
};

#endif // FOKKER_MODBUS_GENENERAL_DEFINITIONS_H_