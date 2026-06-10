
#ifndef FOKKER_MODBUS_CLIENT_INTERFACE_H_
#define FOKKER_MODBUS_CLIENT_INTERFACE_H_

#include "general_modbus_definitions.h"
#include "etl/delegate.h"

    class ModbusClientInterface
    {
    public:
        enum class Result
        {
            NO_ERROR = 0,
            SHIT_HAPPEND,
            TRANSMIT_ERROR,
            RECEIVE_ERROR,
            DECODING_FAILURE,
            ERROR_RESPONSE,
        };
        using DoneCallback = etl::delegate<void(Result)>;

        virtual void Start() = 0;
        virtual void SetNumberOfRetries(uint8_t retries) = 0;
        virtual bool IsBusy() const = 0;

        //  Discretes Input, Single bit,  Read-Only, This type of data can be provided by an I/O system.
        virtual void ReadDiscreteInputs(uint8_t targetAddress,
                                        uint16_t registerAdress,
                                        uint16_t quantity,
                                        uint16_t *outputBuffer,
                                        const DoneCallback &callback) = 0;

        //  Coils, Single bit, Read-Write, This type of data can be alterable by an application program.
        virtual void ReadCoils(uint8_t targetAddress,
                               uint16_t registerAdress,
                               uint16_t quantity, // Number of coils
                               uint16_t *outputBuffer,
                               const DoneCallback &callback) = 0;

        virtual void WriteSingleCoil(uint8_t targetAddress,
                                     uint16_t registerAdress,
                                     bool value,
                                     const DoneCallback &callback) = 0;

        virtual void WriteMultipleCoils(uint8_t targetAddress,
                                        uint16_t registerAdress,
                                        uint16_t quantity,
                                        uint16_t *values, // bytes allowed
                                        const DoneCallback &callback) = 0;
        // Value example: 10 coils starting at coil 20, Then coils order: 27 26 25 24 23 22 21 20 - - - - - - 29 28

        //  Input Registers, 16-bit word, Read-Only, This type of data can be provided by an I/O system
        virtual void ReadInputRegisters(uint8_t targetAddress,
                                        uint16_t registerAdress,
                                        uint16_t quantity,
                                        uint16_t *outputBuffer,
                                        const DoneCallback &callback) = 0;

        //  Holding Registers, 16-bit word, Read-Write, This type of data can be alterable by an application program.
        virtual void ReadHoldingRegisters(uint8_t targetAddress,
                                          uint16_t registerAdress,
                                          uint16_t quantity,
                                          uint16_t *outputBuffer,
                                          const DoneCallback &callback) = 0;

        virtual void WriteSingleRegister(uint8_t targetAddress,
                                         uint16_t registerAdress,
                                         uint16_t value,
                                         const DoneCallback &callback) = 0;

        virtual void WriteMultipleRegisters(uint8_t targetAddress,
                                            uint16_t registerAdress,
                                            uint16_t quantity,
                                            uint16_t *values,
                                            const DoneCallback &callback) = 0;

        // To be added (Optional):
        // 0x07 Read Exception Status (Serial Line only)
        // Request: Function code (1 byte)
        // Response: Function code (1 byte), Output Data (1 byte)

        // 0x08 Diagnostics (Serial Line only)
        // Request: Function code (1 byte), DiagnosticsSubFunctionCodes (2 bytes), Data (2 bytes)
        // Response: Function code (1 byte), DiagnosticsSubFunctionCodes (2 bytes), Data (2 bytes)

        // 0x0B Get Comm Event Counter (Serial Line only)
        // Request: Function code (1 byte)
        // Response: Function code (1 byte), Status (2 bytes), Event Count (2 bytes)

        // 0x0C Get Comm Event Log (Serial Line only)
        // Request: Function code (1 byte)
        // Response: Function code (1 byte), Byte Count (1 byte), Status (2 bytes), Event Count (2 bytes), Message Count (2 bytes), Events (variable)

        // 0x11 Report Server ID (Serial Line only)
        // Request: Function code (1 byte)
        // Response: Function code (1 byte), Byte Count (1 byte), Server ID (variable), Run Indicator Status (1 byte), Additional Data (variable)

        // 0x14 Read File Record.
        // Request: Function code (1 byte), Byte Count (1 byte), FileRecordData. See chapter 6.14
        // Response: Function code (1 byte), Byte Count (1 byte), FileRecordData. See chapter 6.14

        // 0x15 Write File Record
        // Request: Function code (1 byte), Byte Count (1 byte), FileRecordData, Data (N*2 bytes). See chapter 6.15
        // Response: Function code (1 byte), Byte Count (1 byte), FileRecordData, Data (N*2 bytes). See chapter 6.15

        // 0x16 Mask Write Register
        // Request: Function code (1 byte), Reference Address (2 bytes), AND Mask (2 bytes), OR Mask (2 bytes)
        // Response: Function code (1 byte), Reference Address (2 bytes), AND Mask (2 bytes), OR Mask (2 bytes)

        // 0x17 Read/Write Multiple Registers
        // Request: Function code (1 byte), Read Start Address (2 bytes), Read Quantity (2 bytes), Write Start Address (2 bytes), Write Quantity (2 bytes), Write Byte Count (1 byte), Write Data (variable)
        // Response: Function code (1 byte), Byte Count (1 byte), Read Data (variable)

        // 0x18 Read FIFO Queue
        // Request: Function code (1 byte), FIFO Pointer Address (2 bytes)
        // Response: Function code (1 byte), Byte Count (2 bytes), FIFO Count (2 bytes), FIFO Data (N × 2 bytes)

        // 0x2B Encapsulated Interface Transport
        // Request: Function code (1 byte), MEI Type (1 byte), MEI Data (variable)
        // Response: Function code (1 byte), MEI Type (1 byte), MEI Data (variable)

        // 0x2B / 0x0D CANopen General Reference
        // Request: Function code (1 byte), MEI Type = 0x0D (1 byte), CANopen PDU (variable)
        // Response: Function code (1 byte), MEI Type = 0x0D (1 byte), CANopen PDU (variable)

        // 0x2B / 0x0E Read Device Identification
        // Request: Function code (1 byte), MEI Type = 0x0E (1 byte), Read Device ID parameters
        // Response: Function code (1 byte), MEI Type = 0x0E (1 byte), Conformity Level, More Follows, Next Object ID, Object List
    };

#endif // FOKKER_MODBUS_CLIENT_INTERFACE_H_
