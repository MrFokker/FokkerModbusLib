
#ifndef FOKKER_MODBUS_CLIENT_H_
#define FOKKER_MODBUS_CLIENT_H_

#include "modbus_client_interface.h"
#include "modbus_port_interface.h"

#include "etl/array.h"
#include "etl/byte_stream.h"
#include "etl/queue.h"

struct RequestAdministration;

template <typename Framer, size_t QUEUE_SIZE = 1>
class ModbusClient : public ModbusClientInterface
{
  public:
    ModbusClient(ModbusPortInterface& port) : m_port(port) {}

    void Start() override
    {
        m_currentRequests   = {};
        m_nextTransactionId = 0x0000;
        m_port.Start();
    }
    void SetNumberOfRetries(uint8_t retries) override
    {
        m_numberOfRetries = retries;
    }
    bool IsBusy() const override
    {
        return m_currentRequests.m_functionCode != ModbusDefinitions::FunctionCodes::DEFAULT_VALUE;
    }

    //  Discretes Input, Single bit,  Read-Only, This type of data can be provided by an I/O system.
    void ReadDiscreteInputs(uint8_t                                    targetAddress,
                            uint16_t                                   registerAdress,
                            uint16_t                                   quantity,
                            uint16_t*                                  outputBuffer,
                            const ModbusClientInterface::DoneCallback& callback) override
    {
        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = quantity;
        request.m_appdata          = outputBuffer;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

    //  Coils, Single bit, Read-Write, This type of data can be alterable by an application program.
    void ReadCoils(uint8_t                                    targetAddress,
                   uint16_t                                   registerAdress,
                   uint16_t                                   quantity,
                   uint16_t*                                  outputBuffer,
                   const ModbusClientInterface::DoneCallback& callback) override
    {
        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_COILS;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = quantity;
        request.m_appdata          = outputBuffer;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

    void WriteSingleCoil(uint8_t                                    targetAddress,
                         uint16_t                                   registerAdress,
                         bool                                       value,
                         const ModbusClientInterface::DoneCallback& callback) override
    {
        constexpr uint16_t WRITE_SINGLE_COIL_ON_VALUE  = 0xFF00;
        constexpr uint16_t WRITE_SINGLE_COIL_OFF_VALUE = 0x0000;

        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = value ? WRITE_SINGLE_COIL_ON_VALUE : WRITE_SINGLE_COIL_OFF_VALUE;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

    void WriteMultipleCoils(uint8_t                                    targetAddress,
                            uint16_t                                   registerAdress,
                            uint16_t                                   quantity, // Coil count
                            uint16_t*                                  values,
                            const ModbusClientInterface::DoneCallback& callback) override
    {
        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = quantity;
        request.m_appdata          = values;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

    //  Input Registers, 16-bit word, Read-Only, This type of data can be provided by an I/O system
    void ReadInputRegisters(uint8_t                                    targetAddress,
                            uint16_t                                   registerAdress,
                            uint16_t                                   quantity,
                            uint16_t*                                  outputBuffer,
                            const ModbusClientInterface::DoneCallback& callback) override
    {
        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = quantity;
        request.m_appdata          = outputBuffer;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

    //  Holding Registers, 16-bit word, Read-Write, This type of data can be alterable by an
    //  application program.
    void ReadHoldingRegisters(uint8_t                                    targetAddress,
                              uint16_t                                   registerAdress,
                              uint16_t                                   quantity,
                              uint16_t*                                  outputBuffer,
                              const ModbusClientInterface::DoneCallback& callback) override
    {
        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = quantity;
        request.m_appdata          = outputBuffer;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

    void WriteSingleRegister(uint8_t                                    targetAddress,
                             uint16_t                                   registerAdress,
                             uint16_t                                   value,
                             const ModbusClientInterface::DoneCallback& callback) override
    {
        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = value;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

    void WriteMultipleRegisters(uint8_t                                    targetAddress,
                                uint16_t                                   registerAdress,
                                uint16_t                                   quantity, // Register count
                                uint16_t*                                  values,
                                const ModbusClientInterface::DoneCallback& callback) override
    {
        RequestAdministration request;
        request.m_functionCode     = ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS;
        request.m_targetAddress    = targetAddress;
        request.m_registerAddress  = registerAdress;
        request.m_quantity         = quantity;
        request.m_appdata          = values;
        request.m_callback         = callback;
        request.m_transactionId    = ++m_nextTransactionId;
        request.m_retriesRemaining = m_numberOfRetries;

        HandleRequest(request);
    }

  private:
    struct RequestAdministration {
        uint8_t                          m_targetAddress   = 0; // RTU: target address, TCP: unit ID
        ModbusDefinitions::FunctionCodes m_functionCode    = ModbusDefinitions::FunctionCodes::DEFAULT_VALUE;
        uint16_t                         m_registerAddress = 0x0000;
        uint16_t                         m_quantity        = 0; // Write single then quantity is used to store the value
        uint16_t                         m_transactionId   = 0; // Only needed for TCP

        uint8_t m_retriesRemaining = 0;

        uint16_t     m_receiveCount = 0;
        uint16_t*    m_appdata      = nullptr;
        DoneCallback m_callback     = {};
    };

    void HandleRequest(const RequestAdministration& request)
    {
        m_currentRequests = request;

        uint16_t pduSize = BuildRequestPdu(Framer::ADU_BEGIN_SIZE_BYTES, request);

        // Wrap Request
        Framer::BuildAduFrame(m_buffer.data(), pduSize, request.m_transactionId, request.m_targetAddress);

        m_port.Transmit({m_buffer.data(), Framer::ADU_BEGIN_SIZE_BYTES + pduSize + Framer::ADU_END_SIZE_BYTES},
                        ModbusPortInterface::TransmitCallback::create<ModbusClient, &ModbusClient::HandleTransmitComplete>(*this));
    }

    uint16_t BuildRequestPdu(uint8_t pduOffset, const RequestAdministration& request)
    {
        etl::byte_stream_writer writer(m_buffer.data() + pduOffset, m_buffer.end(), etl::endian::big);
        writer.write(static_cast<uint8_t>(request.m_functionCode));
        writer.write_unchecked<uint16_t>(request.m_registerAddress);

        if ((request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_COILS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER)) {
            writer.write_unchecked<uint16_t>(request.m_quantity);
        }

        if (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS) {
            writer.write_unchecked<uint8_t>(static_cast<uint8_t>(request.m_quantity * 2));
            writer.write_unchecked(request.m_appdata, request.m_quantity);
        } else if (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS) {
            // TODO: add data to payload
            writer.write_unchecked<uint16_t>((request.m_quantity + 7) / 8);
        }

        return writer.size_bytes();
    }

    void HandleTransmitComplete(ModbusPortInterface::PortResult result)
    {
        RequestAdministration& request = m_currentRequests;

        if (result == ModbusPortInterface::PortResult::NO_ERROR) {
            m_port.Receive(m_buffer,
                           m_timeoutMs,
                           ModbusPortInterface::ReceiveCallback::create<ModbusClient, &ModbusClient::HandleReceiveComplete>(*this));
        } else {
            m_port.Cleanup();
            if (request.m_retriesRemaining == 0) {
                HandleFinish(Result::TRANSMIT_ERROR);
            } else {
                --request.m_retriesRemaining;
                HandleRequest(request); // TODO: Fix this in case of queue
            }
        }
    }

    void HandleReceiveComplete(ModbusPortInterface::PortResult result, uint16_t length)
    {
        RequestAdministration& request = m_currentRequests;

        if (result != ModbusPortInterface::PortResult::NO_ERROR) {
            m_port.Cleanup();
            if (request.m_retriesRemaining == 0) {
                HandleFinish(Result::RECEIVE_ERROR);
            } else {
                --request.m_retriesRemaining;
                HandleRequest(request); // TODO: Fix this in case of queue
            }
        } else {
            HandleResponse(length);
        }
    }

    void HandleResponse(uint16_t length)
    {
        const RequestAdministration& request = m_currentRequests;

        // Validate Response
        uint8_t pduSize = Framer::ValidateAduFrame(m_buffer.data(), length, request.m_transactionId, request.m_targetAddress);
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
        static constexpr uint8_t     WRITE_RESPONSE_PDU_SIZE = 5;
        const RequestAdministration& request                 = m_currentRequests;
        bool                         success                 = false;
        etl::byte_stream_reader      reader(m_buffer.data() + pduOffset, pduSize, etl::endian::big);

        success = (request.m_functionCode == ModbusDefinitions::ToFunctionCode(reader.read_unchecked<uint8_t>()));

        if ((request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_COILS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_DISCRETE_INPUTS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_HOLDING_REGISTERS) ||
            (request.m_functionCode == ModbusDefinitions::FunctionCodes::READ_INPUT_REGISTERS)) {
            const uint8_t byteCount = reader.read_unchecked<uint8_t>();
            success                 = success && (byteCount % 2 == 0) && (byteCount == (pduSize - 2));
            // success = success && (request.m_quantity == ??)
            for (uint8_t registerIndex = 0; success && registerIndex < (byteCount / 2); ++registerIndex) {
                request.m_appdata[registerIndex] = reader.read_unchecked<uint16_t>();
                // TODO: make it read uint8_t for COILS and Discrete inputs
            }
        } else if ((request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_COILS) ||
                   (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_MULTIPLE_REGISTERS) ||
                   (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_COIL) ||
                   (request.m_functionCode == ModbusDefinitions::FunctionCodes::WRITE_SINGLE_REGISTER)) {
            success = success && (pduSize == WRITE_RESPONSE_PDU_SIZE);
            success = success && (request.m_registerAddress == reader.read_unchecked<uint16_t>());
            success = success && (request.m_quantity == reader.read_unchecked<uint16_t>()); // TODO: fix this for coils
        }
        return success;
    }

    void HandleFinish(Result result)
    {
        DoneCallback callback = m_currentRequests.m_callback;
        m_currentRequests     = {};
        callback.call_if(Result::NO_ERROR);
    }

  private:
    RequestAdministration m_currentRequests;
    // etl::queue<RequestAdministration, QUEUE_SIZE, etl::memory_model::MEMORY_MODEL_SMALL> m_currentRequests = {};

    uint16_t                                  m_nextTransactionId = 0;
    uint16_t                                  m_timeoutMs         = 0;
    uint8_t                                   m_numberOfRetries   = 0;
    etl::array<uint8_t, Framer::MAX_PDU_SIZE> m_buffer;

    ModbusPortInterface& m_port;
};

#endif // FOKKER_MODBUS_CLIENT_H_