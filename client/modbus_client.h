
#ifndef FOKKER_MODBUS_CLIENT_H_
#define FOKKER_MODBUS_CLIENT_H_

#include "modbus_client_interface.h"
#include "modbus_port_interface.h"

#include "etl/array.h"
#include "etl/byte_stream.h"

template <typename Framer> class ModbusClient : public ModbusClientInterface
{
  public:
    ModbusClient(ModbusPortInterface& port) : m_port(port) {}
    void Start() override
    {
        m_currentRequest = RequestAdministration{};
        m_port.Start();
    }
    void SetNumberOfRetries(uint8_t retries) override
    {
        m_numberOfRetries = retries;
    }
    bool IsBusy() const override
    {
        return m_currentRequest.m_functionCode != ModbusDefinitions::FunctionCodes::DEFAULT_VALUE;
    }

    //  Discretes Input, Single bit,  Read-Only, This type of data can be provided by an I/O system.
    void ReadDiscreteInputs(uint8_t                                    targetAddress,
                            uint16_t                                   registerAdress,
                            uint16_t                                   quantity,
                            uint16_t*                                  outputBuffer,
                            const ModbusClientInterface::DoneCallback& callback) override
    {
        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = quantity;
        m_currentRequest.m_appdata          = outputBuffer;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

    //  Coils, Single bit, Read-Write, This type of data can be alterable by an application program.
    void ReadCoils(uint8_t                                    targetAddress,
                   uint16_t                                   registerAdress,
                   uint16_t                                   quantity,
                   uint16_t*                                  outputBuffer,
                   const ModbusClientInterface::DoneCallback& callback) override
    {
        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_COILS;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = quantity;
        m_currentRequest.m_appdata          = outputBuffer;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

    void WriteSingleCoil(uint8_t                                    targetAddress,
                         uint16_t                                   registerAdress,
                         bool                                       value,
                         const ModbusClientInterface::DoneCallback& callback) override
    {
        constexpr uint16_t WRITE_SINGLE_COIL_ON_VALUE  = 0xFF00;
        constexpr uint16_t WRITE_SINGLE_COIL_OFF_VALUE = 0x0000;

        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = value ? WRITE_SINGLE_COIL_ON_VALUE : WRITE_SINGLE_COIL_OFF_VALUE;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

    void WriteMultipleCoils(uint8_t                                    targetAddress,
                            uint16_t                                   registerAdress,
                            uint16_t                                   quantity, // Coil count
                            uint16_t*                                  values,
                            const ModbusClientInterface::DoneCallback& callback) override
    {
        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = quantity;
        m_currentRequest.m_appdata          = values;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

    //  Input Registers, 16-bit word, Read-Only, This type of data can be provided by an I/O system
    void ReadInputRegisters(uint8_t                                    targetAddress,
                            uint16_t                                   registerAdress,
                            uint16_t                                   quantity,
                            uint16_t*                                  outputBuffer,
                            const ModbusClientInterface::DoneCallback& callback) override
    {
        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = quantity;
        m_currentRequest.m_appdata          = outputBuffer;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

    //  Holding Registers, 16-bit word, Read-Write, This type of data can be alterable by an
    //  application program.
    void ReadHoldingRegisters(uint8_t                                    targetAddress,
                              uint16_t                                   registerAdress,
                              uint16_t                                   quantity,
                              uint16_t*                                  outputBuffer,
                              const ModbusClientInterface::DoneCallback& callback) override
    {
        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = quantity;
        m_currentRequest.m_appdata          = outputBuffer;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

    void WriteSingleRegister(uint8_t                                    targetAddress,
                             uint16_t                                   registerAdress,
                             uint16_t                                   value,
                             const ModbusClientInterface::DoneCallback& callback) override
    {
        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = value;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

    void WriteMultipleRegisters(uint8_t                                    targetAddress,
                                uint16_t                                   registerAdress,
                                uint16_t                                   quantity, // Register count
                                uint16_t*                                  values,
                                const ModbusClientInterface::DoneCallback& callback) override
    {
        m_currentRequest.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS;
        m_currentRequest.m_targetAddress    = targetAddress;
        m_currentRequest.m_registerAddress  = registerAdress;
        m_currentRequest.m_retriesRemaining = m_numberOfRetries;
        m_currentRequest.m_quantity         = quantity;
        m_currentRequest.m_appdata          = values;
        m_currentRequest.m_callback         = callback;
        HandleRequest();
    }

  private:
    void HandleRequest()
    {
        uint16_t pduSize = BuildRequestPdu(Framer::ADU_BEGIN_SIZE_BYTES);

        // Wrap Request
        Framer::BuildAduFrame(m_buffer.data(), pduSize, m_currentRequest.m_transactionId, m_currentRequest.m_targetAddress);

        m_port.Transmit({m_buffer.data(), Framer::ADU_BEGIN_SIZE_BYTES + pduSize + Framer::ADU_END_SIZE_BYTES},
                        ModbusPortInterface::TransmitCallback::create<ModbusClient, &ModbusClient::HandleTransmitComplete>(*this));
    }

    uint16_t BuildRequestPdu(uint8_t pduOffset)
    {
        etl::byte_stream_writer writer(m_buffer.data() + pduOffset, m_buffer.end(), etl::endian::big);
        writer.write(static_cast<uint8_t>(m_currentRequest.m_functionCode));
        writer.write_unchecked<uint16_t>(m_currentRequest.m_registerAddress);

        if ((m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_COILS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER)) {
            writer.write_unchecked<uint16_t>(m_currentRequest.m_quantity);
        }

        if (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS) {
            writer.write_unchecked<uint8_t>(static_cast<uint8_t>(m_currentRequest.m_quantity * 2));
            writer.write_unchecked(m_currentRequest.m_appdata, m_currentRequest.m_quantity);
        } else if (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS) {
            // TODO: add data to payload
            writer.write_unchecked<uint16_t>((m_currentRequest.m_quantity + 7) / 8);
        }

        return writer.size_bytes();
    }

    void HandleTransmitComplete(ModbusPortInterface::PortResult result)
    {
        if (result == ModbusPortInterface::PortResult::NO_ERROR) {
            m_port.Receive(m_buffer,
                           m_timeoutMs,
                           ModbusPortInterface::ReceiveCallback::create<ModbusClient, &ModbusClient::HandleReceiveComplete>(*this));
        } else {
            m_port.Cleanup();
            if (m_currentRequest.m_retriesRemaining == 0) {
                m_currentRequest.m_callback.call_if(Result::TRANSMIT_ERROR);
            } else {
                --m_currentRequest.m_retriesRemaining;
                HandleRequest();
            }
        }
    }

    void HandleReceiveComplete(ModbusPortInterface::PortResult result, uint16_t length)
    {
        if (result != ModbusPortInterface::PortResult::NO_ERROR) {
            m_port.Cleanup();
            if (m_currentRequest.m_retriesRemaining == 0) {
                HandleFinish(Result::RECEIVE_ERROR);
            } else {
                --m_currentRequest.m_retriesRemaining;
                HandleRequest();
            }
        } else {
            HandleResponse(length);
        }
    }

    void HandleResponse(uint16_t length)
    {
        // Validate Response
        uint8_t                           pduSize = Framer::ValidateAduFrame(m_buffer.data(), length, 0, m_currentRequest.m_targetAddress);
        ModbusDefinitions::ExceptionCodes errorCode = HandleErrorResponse(Framer::ADU_BEGIN_SIZE_BYTES);
        if (pduSize == 0) {
            // Transport error
            // TODO: Add retry, because of data corruption
            HandleFinish(Result::SHIT_HAPPEND);
        } else if (errorCode != ModbusDefinitions::ExceptionCodes::DEFAULT_VALUE) {
            // Return Error code
            HandleFinish(Result::ERROR_RESPONSE);
        } else if (!ParsePduFrame(Framer::ADU_BEGIN_SIZE_BYTES, pduSize)) {
            // PDU error
            HandleFinish(Result::DECODING_FAILURE);
        } else {
            // Successfully parced
            HandleFinish(Result::NO_ERROR);
        }
    }

    ModbusDefinitions::ExceptionCodes HandleErrorResponse(uint8_t pduOffset) const
    {
        ModbusDefinitions::ExceptionCodes error = ModbusDefinitions::ExceptionCodes::DEFAULT_VALUE;
        if (ModbusDefinitions::IsErrorResponse(m_buffer[pduOffset])) {
            error = static_cast<ModbusDefinitions::ExceptionCodes>(m_buffer[pduOffset + 1]);
        }
        return error;
    }

    bool ParsePduFrame(uint8_t pduOffset, uint8_t pduSize)
    {
        bool                    success = false;
        etl::byte_stream_reader reader(m_buffer.data() + pduOffset, pduSize, etl::endian::big);

        success = (m_currentRequest.m_functionCode == ModbusDefinitions::ToFunctionCode(reader.read_unchecked<uint8_t>()));

        if ((m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_COILS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS) ||
            (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS)) {
            const uint8_t byteCount = reader.read_unchecked<uint8_t>();
            success                 = success && (byteCount % 2 == 0) && (byteCount == (pduSize - 2));
            // success = success && (m_currentRequest.m_quantity == ??)
            for (uint8_t registerIndex = 0; success && registerIndex < (byteCount / 2); ++registerIndex) {
                m_currentRequest.m_appdata[registerIndex] = reader.read_unchecked<uint16_t>();
                // TODO make it read uint8_t for COILS and Discrete inputs
            }
        } else if ((m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS) ||
                   (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS) ||
                   (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL) ||
                   (m_currentRequest.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER)) {
            static constexpr uint8_t WRITE_RESPONSE_PDU_SIZE = 5;
            success                                          = success && (pduSize == WRITE_RESPONSE_PDU_SIZE);
            success = success && (m_currentRequest.m_registerAddress == reader.read_unchecked<uint16_t>());
            success = success && (m_currentRequest.m_quantity == reader.read_unchecked<uint16_t>()); // TODO fix this for coils
        }
        return success;
    }

    void HandleFinish(Result result)
    {
        DoneCallback callback = m_currentRequest.m_callback;
        m_currentRequest      = {};
        callback.call_if(Result::NO_ERROR);
    }

    struct RequestAdministration {
        uint8_t                          m_targetAddress   = 0; // RTU: target address, TCP: unit ID
        ModbusDefinitions::FunctionCodes m_functionCode    = ModbusDefinitions::FunctionCodes::DEFAULT_VALUE;
        uint16_t                         m_registerAddress = 0x00;
        uint16_t                         m_quantity        = 0; // Write single then quantity is used to store the value
        uint16_t                         m_transactionId   = 0; // Only needed for TCP, TODO(add setter)
        uint16_t                         m_protocolId      = 0; // Only needed for TCP, constant?

        uint8_t m_retriesRemaining = 0;

        uint16_t     m_receiveCount = 0;
        uint16_t*    m_appdata      = nullptr;
        DoneCallback m_callback     = {};
    } m_currentRequest = {};

    uint16_t                                  m_timeoutMs       = 0;
    uint8_t                                   m_numberOfRetries = 0;
    etl::array<uint8_t, Framer::MAX_PDU_SIZE> m_buffer;

    ModbusPortInterface& m_port;
};

#endif // FOKKER_MODBUS_CLIENT_H_