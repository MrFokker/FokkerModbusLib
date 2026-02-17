
#ifndef FOKKER_MODBUS_CLIENT_H_
#define FOKKER_MODBUS_CLIENT_H_

#include "modbus_client_interface.h"
#include "modbus_port_interface.h"

#include "etl/array.h"

template <typename Framer>
class ModbusClient : public ModbusClientInterface
{
public:
    ModbusClient(ModbusPortInterface &port);

    void Start();
    void SetNumberOfRetries(uint8_t retries);

    // --- Read Functions ---
    // void ReadCoils(uint8_t targetAddress,
    //                uint16_t registerAdress,
    //                uint16_t quantity,
    //                uint8_t *outputBuffer,
    //                const ModbusClientInterface::DoneCallback &callback) override;

    // void ReadDiscreteInputs(uint8_t targetAddress,
    //                         uint16_t registerAdress,
    //                         uint16_t quantity,
    //                         uint8_t *outputBuffer,
    //                         const ModbusClientInterface::DoneCallback &callback) override;

    void ReadHoldingRegisters(uint8_t targetAddress,
                              uint16_t registerAdress,
                              uint16_t quantity,
                              uint16_t *outputBuffer,
                              const ModbusClientInterface::DoneCallback &callback) override;

    void ReadInputRegisters(uint8_t targetAddress,
                            uint16_t registerAdress,
                            uint16_t quantity,
                            uint16_t *outputBuffer,
                            const ModbusClientInterface::DoneCallback &callback) override;

    // --- Write Functions (Single) ---
    void WriteSingleCoil(uint8_t targetAddress,
                         uint16_t registerAdress,
                         bool value,
                         const ModbusClientInterface::DoneCallback &callback) override;

    void WriteSingleRegister(uint8_t targetAddress,
                             uint16_t registerAdress,
                             uint16_t value,
                             const ModbusClientInterface::DoneCallback &callback) override;

    // --- Write Functions (Multiple) ---
    // void WriteMultipleCoils(uint8_t targetAddress,
    //                         uint16_t registerAdress,
    //                         const uint8_t *values,
    //                         uint16_t quantity,
    //                         const ModbusClientInterface::DoneCallback &callback) override;

    void WriteMultipleRegisters(uint8_t targetAddress,
                                uint16_t registerAdress,
                                const uint16_t *values,
                                uint16_t quantity,
                                const ModbusClientInterface::DoneCallback &callback) override;

    // --- Optional Diagnostics ---
    void Diagnostics(uint8_t targetAddress,
                     uint16_t subFunction,
                     uint16_t data,
                     const ModbusClientInterface::DoneCallback &callback) override;

private:
    void HandleRequest()
    {
        uint16_t pduSize = BuildPduFrame(Framer::ADU_START_SIZE_BYTES);

        // Wrap response
        Framer::BuildAduFrame(m_currentRequest.m_targetAddress, m_buffer.data(), pduSize);
        m_port.Transmit({m_buffer.data(), Framer::ADU_START_SIZE_BYTES + Framer::ADU_END_SIZE_BYTES + pduSize}, {});
    }
    void HandleResponse(uint16_t length)
    {
        if (!Framer::ValidateAduFrame(m_buffer.data(), length))
        {
        }

        // Parse PDU (shared)
        ParsePduFrame(m_buffer)
    }
    uint16_t BuildPduFrame(uint8_t pduOffset);
    void ParsePduFrame(uint8_t pduOffset);

    void HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t);
    void HandleTransmitComplete(ModbusPortInterface::PortResult);

    struct RequestAdministration
    {
        uint8_t m_targetAddress = 0;
        uint8_t m_functionCode = 0x00;
        uint16_t m_registerAddress = 0x00;
        uint16_t m_byteCount = 0;
        uint16_t m_transactionId = 0; // Only needed for TCP
        uint16_t m_protocolId = 0;    // Only needed for

        uint16_t m_timeoutMs = 0;
        uint8_t m_retriesRemaining = 0;

        uint16_t m_receiveCount = 0;
        uint8_t *m_appdata = nullptr;
        DoneCallback m_callback = {};
    } m_currentRequest = {};

    uint8_t m_numberOfRetries = 0;
    etl::array<uint8_t, Framer::TOTAL_BUFFER_SIZE> m_buffer;

    ModbusPortInterface &m_port;
};

#endif // FOKKER_MODBUS_CLIENT_H_