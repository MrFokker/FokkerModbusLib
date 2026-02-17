#include "modbus_client.h"

#include "etl/byte_stream.h"

namespace {
    constexpr uint16_t WRITE_SINGLE_COIL_ON_VALUE = 0xFF00;
    constexpr uint16_t WRITE_SINGLE_COIL_OFF_VALUE = 0x0000;
}

ModbusClient::ModbusClient(ModbusPortInterface &port)
    : m_port(port){
}

void ModbusClient::Start()
{
    m_port.Start();
}
void ModbusClient::SetNumberOfRetries(uint8_t retries)
{
    m_retries = retries;
}

// --- Read Functions ---
// void ModbusClient::ReadCoils(uint8_t targetAddress,
//                              uint16_t registerAdress,
//                              uint16_t quantity,
//                              uint8_t *outputBuffer,
//                              const ModbusClientInterface::DoneCallback &callback)
// {
//     if (m_currentRequest != RequestAdministration{})
//     {
//         // return DEVICE_BUSY_ERROR;
//     }
//     m_currentRequest.m_targetAddress = targetAddress;
//     m_currentRequest.m_registerAddress = registerAdress;
//     m_currentRequest.m_byteCount = 2 *quantity;
//     m_currentRequest.m_appdata = outputBuffer;
//     m_currentRequest.m_callback = callback;
//     HandleRequest();
// }

// void ModbusClient::ReadDiscreteInputs(uint8_t targetAddress,
//                                       uint16_t registerAdress,
//                                       uint16_t quantity,
//                                       uint8_t *outputBuffer,
//                                       const ModbusClientInterface::DoneCallback &callback)
// {
//     if (m_currentRequest != RequestAdministration{})
//     {
//         // return DEVICE_BUSY_ERROR;
//     }
//     m_currentRequest.m_targetAddress = targetAddress;
//     m_currentRequest.m_registerAddress = registerAdress;
//     m_currentRequest.m_byteCount = 2 *quantity;
//     m_currentRequest.m_appdata = outputBuffer;
//     m_currentRequest.m_callback = callback;
//     HandleRequest();
// }

void ModbusClient::ReadHoldingRegisters(uint8_t targetAddress,
                                        uint16_t registerAdress,
                                        uint16_t quantity,
                                        uint16_t *outputBuffer,
                                        const ModbusClientInterface::DoneCallback &callback)
{
    // if (m_currentRequest != RequestAdministration{})
    // {
    //     // return DEVICE_BUSY_ERROR;
    // }
    m_currentRequest.m_targetAddress = targetAddress;
    m_currentRequest.m_registerAddress = registerAdress;
    m_currentRequest.m_byteCount = 2 *quantity;
    m_currentRequest.m_appdata = outputBuffer;
    m_currentRequest.m_callback = callback;
    HandleRequest();
}

void ModbusClient::ReadInputRegisters(uint8_t targetAddress,
                                      uint16_t registerAdress,
                                      uint16_t quantity,
                                      uint16_t *outputBuffer,
                                      const ModbusClientInterface::DoneCallback &callback)
{
    // if (m_currentRequest != RequestAdministration())
    // {
    //     return DEVICE_BUSY_ERROR;
    // }
    m_currentRequest.m_targetAddress = targetAddress;
    m_currentRequest.m_registerAddress = registerAdress;
    m_currentRequest.m_byteCount = 2 *quantity;
    m_currentRequest.m_appdata = outputBuffer;
    m_currentRequest.m_callback = callback;
    HandleRequest();
}

// --- Write Functions (Single) ---
void ModbusClient::WriteSingleCoil(uint8_t targetAddress,
                                   uint16_t registerAdress,
                                   bool value,
                                   const ModbusClientInterface::DoneCallback &callback)
{
    // if (m_currentRequest != RequestAdministration{})
    // {
    //     // return DEVICE_BUSY_ERROR;
    // }
    m_currentRequest.m_targetAddress = targetAddress;
    m_currentRequest.m_registerAddress = registerAdress;
    m_currentRequest.m_byteCount = 2 *1;
    m_currentRequest.m_appdata = value ? &WRITE_SINGLE_COIL_ON_VALUE : &WRITE_SINGLE_COIL_OFF_VALUE;
    m_currentRequest.m_callback = callback;
    HandleRequest();
}

void ModbusClient::WriteSingleRegister(uint8_t targetAddress,
                                       uint16_t registerAdress,
                                       uint16_t value,
                                       const ModbusClientInterface::DoneCallback &callback)
{
    // if (m_currentRequest != RequestAdministration{})
    // {
    //     // return DEVICE_BUSY_ERROR;
    // }
    m_currentRequest.m_targetAddress = targetAddress;
    m_currentRequest.m_registerAddress = registerAdress;
    m_currentRequest.m_byteCount = 2 *1;
    m_currentRequest.m_appdata = value;
    m_currentRequest.m_callback = callback;
    HandleRequest();
}

// --- Write Functions (Multiple) ---
// void ModbusClient::WriteMultipleCoils(uint8_t targetAddress,
//                                       uint16_t registerAdress,
//                                       const uint16_t *values,
//                                       uint16_t quantity,
//                                       const ModbusClientInterface::DoneCallback &callback)
// {
//     if (m_currentRequest != RequestAdministration{})
//     {
//         // return DEVICE_BUSY_ERROR;
//     }
//     m_currentRequest.m_targetAddress = targetAddress;
//     m_currentRequest.m_registerAddress = registerAdress;
//     m_currentRequest.m_byteCount = 2 *quantity;
//     m_currentRequest.m_appdata = values;
//     m_currentRequest.m_callback = callback;
//     HandleRequest();
// }

void ModbusClient::WriteMultipleRegisters(uint8_t targetAddress,
                                          uint16_t registerAdress,
                                          const uint16_t *values,
                                          uint16_t quantity,
                                          const ModbusClientInterface::DoneCallback &callback)
{
    // if (m_currentRequest != RequestAdministration{})
    // {
    //     // return DEVICE_BUSY_ERROR;
    // }
    m_currentRequest.m_targetAddress = targetAddress;
    m_currentRequest.m_registerAddress = registerAdress;
    m_currentRequest.m_byteCount = 2 *quantity;
    m_currentRequest.m_appdata = values;
    m_currentRequest.m_callback = callback;
    HandleRequest();
}

// --- Optional Diagnostics ---
void ModbusClient::Diagnostics(uint8_t targetAddress,
                               uint16_t subFunction,
                               uint16_t data,
                               const ModbusClientInterface::DoneCallback &callback)
{
    if (m_currentRequest != RequestAdministration{})
    {
        // return DEVICE_BUSY_ERROR;
    }
    m_currentRequest.m_targetAddress = targetAddress;
    m_currentRequest.m_registerAddress = registerAdress;
    m_currentRequest.m_byteCount = 2 *quantity;
    m_currentRequest.m_appdata = outputBuffer;
    m_currentRequest.m_callback = callback;
    HandleRequest();
}

uint16_t ModbusClient::BuildPduFrame(uint16_t aduOffset)
{
    etl::byte_stream_writer writer{m_buffer.data() + aduOffset, m_buffer.end()};
    writer.write(static_cast<uint8_t>(m_currentRequest.m_functionCode));
    return writer.size_bytes();
}

void ModbusClient::ParsePduFrame(uint16_t aduOffset)
{
}

void ModbusClient::HandleReceiveComplete(ModbusPortInterface::PortResult, uint16_t length)
{
    HandleResponse(uint16_t length)
    

}

void ModbusClient::HandleTransmitComplete(ModbusPortInterface::PortResult)
{
    // Framer::Encode(m_buffer, length);

}