
#ifndef FOKKER_MODBUS_CLIENT_H_
#define FOKKER_MODBUS_CLIENT_H_

#include "modbus_client_interface.h"
#include "modbus_port_interface.h"

#include "etl/array.h"
#include "etl/byte_stream.h"

    template <typename Framer>
    class ModbusClient : public ModbusClientInterface
    {
    public:
        ModbusClient(ModbusPortInterface &port)
            : m_port(port)
        {
        }
        void Start() override
        {
            m_port.Start();
        }
        void SetNumberOfRetries(uint8_t retries) override
        {
            m_numberOfRetries = retries;
        }
        bool IsBusy() const override
        {
            return m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::DEFAULT_VALUE;
        }

        //  Discretes Input, Single bit,  Read-Only, This type of data can be provided by an I/O system.
        void ReadDiscreteInputs(uint8_t targetAddress,
                                uint16_t registerAdress,
                                uint16_t quantity,
                                uint16_t *outputBuffer,
                                const ModbusClientInterface::DoneCallback &callback) override
        {
            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = quantity;
            m_currentRequest.m_appdata = outputBuffer;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

        //  Coils, Single bit, Read-Write, This type of data can be alterable by an application program.
        void ReadCoils(uint8_t targetAddress,
                       uint16_t registerAdress,
                       uint16_t quantity,
                       uint16_t *outputBuffer,
                       const ModbusClientInterface::DoneCallback &callback) override
        {
            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::READ_COILS;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = quantity;
            m_currentRequest.m_appdata = outputBuffer;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

        void WriteSingleCoil(uint8_t targetAddress,
                             uint16_t registerAdress,
                             bool value,
                             const ModbusClientInterface::DoneCallback &callback) override
        {
            constexpr uint16_t WRITE_SINGLE_COIL_ON_VALUE = 0xFF00;
            constexpr uint16_t WRITE_SINGLE_COIL_OFF_VALUE = 0x0000;

            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = value ? WRITE_SINGLE_COIL_ON_VALUE : WRITE_SINGLE_COIL_OFF_VALUE;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

        void WriteMultipleCoils(uint8_t targetAddress,
                                uint16_t registerAdress,
                                uint16_t quantity, // Coil count
                                uint16_t *values,
                                const ModbusClientInterface::DoneCallback &callback) override
        {
            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = quantity;
            m_currentRequest.m_appdata = values;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

        //  Input Registers, 16-bit word, Read-Only, This type of data can be provided by an I/O system
        void ReadInputRegisters(uint8_t targetAddress,
                                uint16_t registerAdress,
                                uint16_t quantity,
                                uint16_t *outputBuffer,
                                const ModbusClientInterface::DoneCallback &callback) override
        {
            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = quantity;
            m_currentRequest.m_appdata = outputBuffer;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

        //  Holding Registers, 16-bit word, Read-Write, This type of data can be alterable by an application program.
        void ReadHoldingRegisters(uint8_t targetAddress,
                                  uint16_t registerAdress,
                                  uint16_t quantity,
                                  uint16_t *outputBuffer,
                                  const ModbusClientInterface::DoneCallback &callback) override
        {
            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = quantity;
            m_currentRequest.m_appdata = outputBuffer;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

        void WriteSingleRegister(uint8_t targetAddress,
                                 uint16_t registerAdress,
                                 uint16_t value,
                                 const ModbusClientInterface::DoneCallback &callback) override
        {
            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = value;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

        void WriteMultipleRegisters(uint8_t targetAddress,
                                    uint16_t registerAdress,
                                    uint16_t quantity, // Register count
                                    uint16_t *values,
                                    const ModbusClientInterface::DoneCallback &callback) override
        {
            m_currentRequest.m_functionCode = ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS;
            m_currentRequest.m_targetAddress = targetAddress;
            m_currentRequest.m_registerAddress = registerAdress;
            m_currentRequest.m_quantity = quantity;
            m_currentRequest.m_appdata = values;
            m_currentRequest.m_callback = callback;
            HandleRequest();
        }

    private:
        void HandleRequest()
        {
            uint16_t pduSize = BuildRequestPdu(Framer::ADU_BEGIN_SIZE_BYTES);

            // Wrap Request
            Framer::BuildAduFrame(m_currentRequest.m_targetAddress, m_buffer.data(), pduSize);
            m_port.Transmit({m_buffer.data(), Framer::ADU_BEGIN_SIZE_BYTES + Framer::ADU_END_SIZE_BYTES + pduSize}, {});
        }

        uint16_t BuildRequestPdu(uint8_t pduOffset)
        {
            etl::byte_stream_writer writer(m_buffer.data() + pduOffset, m_buffer.end(), etl::endian::big);
            writer.write(static_cast<uint8_t>(m_currentRequest.m_functionCode));
            writer.write_unchecked<uint16_t>(m_currentRequest.m_registerAddress);

            if ((m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_COILS) || (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS) ||
                (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS) || (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS) ||
                (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS) || (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS) ||
                (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL) || (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER))
            {
                writer.write_unchecked<uint16_t>(m_currentRequest.m_quantity);
            }

            if (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS)
            {
                writer.write_unchecked<uint8_t>(static_cast<uint8_t>(m_currentRequest.m_quantity * 2));
                writer.write_unchecked(m_currentRequest.m_appdata, m_currentRequest.m_quantity);
            }
            else if (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS)
            {
                writer.write_unchecked<uint16_t>((m_currentRequest.m_quantity + 7) / 8);
            }

            return writer.size_bytes();
        }

        void HandleResponse(uint16_t length)
        {
            // Validate Response
            uint8_t pduSize = Framer::ValidateAduFrame(m_buffer.data(), length) if (pduSize == 0)
            {
                // Transport error
            }

            // Parse PDU (shared)
            ParsePduFrame(m_buffer)
        }
        void ParsePduFrame(uint8_t pduOffset);

        void HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t length)
        {
            HandleResponse(uint16_t length);
        }

        void HandleTransmitComplete(ModbusPortInterface::PortResult)
        {
            // Framer::Encode(m_buffer, length);
        }

        struct RequestAdministration
        {
            uint8_t m_targetAddress = 0;
            ModbusDefinitions::FunctionCodes m_functionCode = ModbusDefinitions::FunctionCodes::DEFAULT_VALUE;
            uint16_t m_registerAddress = 0x00;
            uint16_t m_quantity = 0;      // Write single then quantity is used to store the value
            uint16_t m_transactionId = 0; // Only needed for TCP
            uint16_t m_protocolId = 0;    // Only needed for

            uint8_t m_retriesRemaining = 0;

            uint16_t m_receiveCount = 0;
            uint16_t *m_appdata = nullptr;
            DoneCallback m_callback = {};
        } m_currentRequest = {};

        uint16_t m_timeoutMs = 0;
        uint8_t m_numberOfRetries = 0;
        etl::array<uint8_t, Framer::MAX_PDU_SIZE> m_buffer;

        ModbusPortInterface &m_port;
    };

#endif // FOKKER_MODBUS_CLIENT_H_