#include "TCP_framer.h"
#include "modbus_client.h"
#include "modbus_port_mock.h"

#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;

class TcpClientTestFixture : public ::testing::Test
{
  public:
    size_t                   m_capturedSize = 0;
    etl::array<uint8_t, 256> m_capturedBuffer{};

    ::testing::StrictMock<ModbusRtuPortMock> m_portMock;
    ModbusClient<TCPFramer>                  m_client{m_portMock};

    TcpClientTestFixture() : m_client(m_portMock)
    {
        ON_CALL(m_portMock, Transmit(_, _))
            .WillByDefault(DoAll(
                [this](etl::array_view<uint8_t> tx, const auto&) {
                    m_capturedSize = tx.size();
                    std::copy(tx.begin(), tx.end(), m_capturedBuffer.begin());
                },
                Return(true)));
    }

    void VerifyTransmitData(const uint8_t* expectedFrame, size_t expectedSize)
    {
        for (size_t i = 0; i < m_capturedSize; ++i) {
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(m_capturedBuffer[i]) << " ";
        }
        std::cout << std::dec << std::endl;

        ASSERT_EQ(m_capturedSize, expectedSize);

        for (size_t i = 0; i < expectedSize; ++i) {
            EXPECT_EQ(m_capturedBuffer[i], expectedFrame[i]);
        }
    }
};

TEST_F(TcpClientTestFixture, ReadHoldingRegisters_Busy_after_request_send)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));
    uint16_t output[2] = {};

    EXPECT_FALSE(m_client.IsBusy());

    m_client.ReadHoldingRegisters(0x01, 0x0010, 2, output, {});

    EXPECT_TRUE(m_client.IsBusy());
}

TEST_F(TcpClientTestFixture, ReadHoldingRegisters_Request1)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output[2] = {};

    m_client.ReadHoldingRegisters(0x01, 0x0010, 2, output, {});

    // Transaction: 0x0001
    // Protocol:   0x0000
    // Length:     0x0006
    // Unit:       0x01
    // Function:   0x03
    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x00, 0x10, 0x00, 0x02};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}

TEST_F(TcpClientTestFixture, ReadHoldingRegisters_Request2)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output[4] = {};

    m_client.ReadHoldingRegisters(0x02, 0x0020, 4, output, {});

    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x02, 0x03, 0x00, 0x20, 0x00, 0x04};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}

TEST_F(TcpClientTestFixture, ReadCoils_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output = 0;

    m_client.ReadCoils(0x02, 0x0020, 4, &output, {});

    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x02, 0x01, 0x00, 0x20, 0x00, 0x04};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}

TEST_F(TcpClientTestFixture, ReadDiscreteInputs_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output = 0;

    m_client.ReadDiscreteInputs(0x02, 0x0020, 4, &output, {});

    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x02, 0x02, 0x00, 0x20, 0x00, 0x04};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}

TEST_F(TcpClientTestFixture, ReadInputRegisters_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output[4] = {};

    m_client.ReadInputRegisters(0x02, 0x0020, 4, output, {});

    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x02, 0x04, 0x00, 0x20, 0x00, 0x04};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}

TEST_F(TcpClientTestFixture, WriteSingleCoil_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    m_client.WriteSingleCoil(0x02, 0x0020, true, {});

    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x02, 0x05, 0x00, 0x20, 0xFF, 0x00};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}

TEST_F(TcpClientTestFixture, WriteSingleRegister_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    m_client.WriteSingleRegister(0x02, 0x0020, 0x1234, {});

    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x02, 0x06, 0x00, 0x20, 0x12, 0x34};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}

TEST_F(TcpClientTestFixture, WriteMultipleRegisters_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t values[2] = {0x1234, 0x5678};

    m_client.WriteMultipleRegisters(0x02, 0x0020, 2, values, {});

    // Length = Unit + Function + Addr + Quantity + ByteCount + Data
    //        = 1 + 1 + 2 + 2 + 1 + 4 = 11 = 0x000B
    const uint8_t expectedFrame[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x02, 0x10, 0x00, 0x20, 0x00, 0x02, 0x04, 0x12, 0x34, 0x56, 0x78};

    VerifyTransmitData(expectedFrame, sizeof(expectedFrame));
}