// Functions that can be used by both Modbus Clients and Servers
// Not all functions have to be used by both

#ifndef FOKKER_MODBUS_GENERAL_FUNCTIONS
#define FOKKER_MODBUS_GENERAL_FUNCTIONS

#include "etl/array_view.h"
#include "etl/optional.h"

namespace
{
    constexpr size_t MAX_PDU_MODBUS_SIZE = 256;

    constexpr uint16_t MSB(uint16_t value)
    {
        return value & 0xFF00;
    }
    constexpr uint16_t LSB(uint16_t value)
    {
        return value & 0x00FF;
    }

} // namespace

class ModbusHelperClass
{
  public:
    static constexpr size_t      MODBUS_MAX_FRAME_SIZE_BYTES = 260;
    static constexpr etl::endian BUFFER_ENDIAN               = etl::endian::big;

    using BufferDataType = uint8_t;
    using ReceiveBuffer  = etl::array<BufferDataType, MODBUS_MAX_FRAME_SIZE_BYTES>;
    using SentBuffer     = ReceiveBuffer;
    using ReceiveCount   = uint16_t;
    using AppDataView    = etl::array_view<const BufferDataType>;

    struct RequestAdministration {
        uint8_t  m_targetAddress   = 0;
        uint8_t  m_functionCode    = 0x00;
        uint16_t m_registerAddress = 0x00;
        uint16_t m_registerCount   = 0;
        uint16_t m_transactionId   = 0; // Only needed for TCP
        uint16_t m_protocolId      = 0; // Only needed for

        uint16_t               m_timeoutMs        = 0;
        etl::optional<uint8_t> m_retriesRemaining = {};

        ReceiveCount m_receiveCount = 0;
        AppDataView  m_appdata;
    };

  protected:
    ModbusHelperClass(const ReceiveBuffer& receiveBuffer, ReceiveBuffer& sentBuffer)
        : m_reader(receiveBuffer.begin(), receiveBuffer.end(), BUFFER_ENDIAN), m_writer(sentBuffer.begin(), sentBuffer.end(), BUFFER_ENDIAN)
    {
    }

    void ResetByteStream()
    {
        m_reader.restart();
        m_writer.restart();
    }

    void NewRequest(const RequestAdministration& request)
    {
        m_request = request;
        ResetByteStream();
    }
    void NewRequest(const ReceiveCount receiveCount)
    {
        if (receiveCount <= MODBUS_MAX_FRAME_SIZE_BYTES) {
            m_request = {};
            ResetByteStream();
        }
    }
    RequestAdministration GetRequestAdministration() const
    {
        return m_request;
    }

    void AddTargetAddress()
    {
        m_writer.write<uint8_t>(m_request.m_targetAddress);
    }
    bool ValidateTargetAddress()
    {
        return m_reader.read_unchecked<uint8_t>() == m_request.m_targetAddress;
    }

    void AddFunctionCode()
    {
        m_writer.write<uint8_t>(m_request.m_functionCode);
    }
    bool ValidateFunctionCode()
    {
        return m_reader.read_unchecked<uint8_t>() == m_request.m_functionCode;
    }
    void AddRegisterAddress()
    {
        m_writer.write_unchecked<uint16_t>(m_request.m_registerAddress);
    }
    bool ValidateRegisterAddress()
    {
        return m_reader.read_unchecked<uint16_t>() == m_request.m_registerAddress;
    }

    void AddRegisterCount()
    {
        m_writer.write_unchecked<uint8_t>(m_request.m_appdata.size());
    }
    void GetRegisterCount()
    {
        m_request.m_registerCount = m_reader.read_unchecked<uint16_t>();
    }

    // bool ValidateRegisterCount()
    // {
    //     return m_reader.read<uint16_t> == m_request.m_receiveCount;
    // };
    void AddAppData()
    {
        m_writer.write<BufferDataType>(m_request.m_appdata.begin(), m_request.m_appdata.size());
    }
    void GetAppData()
    {
        m_reader.restart(m_reader.size_bytes() + (m_request.m_registerCount * 2));
        // m_request.m_appdata = {m_reader.end(), m_reader.end() +  m_request.m_registerCount};
    }

    uint16_t CalculateCrc(const etl::array_view<const BufferDataType>& view)
    {
        (void)view;
        return 0;
    }
    void AddCrc()
    {
        // m_writer.write_unchecked<uint16_t>(CalculateCrc({m_writer.begin(), m_writer.end()}));
    }
    bool ValidateCrc()
    {
        // return CalculateCrc({}) == 0;
        return false;
    }

  private:
    etl::byte_stream_reader m_reader;
    etl::byte_stream_writer m_writer;
    RequestAdministration   m_request;
};

#endif // FOKKER_MODBUS_SHARED_FUNCTIONS