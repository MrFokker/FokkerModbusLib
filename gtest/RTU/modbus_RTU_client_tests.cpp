#include "modbus_client.h"
#include "RTU_framer.h"
#include "modbus_port_mock.h"

#include <iostream>
#include <iomanip> // for std::hex and std::setw
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;

class RtuClientTestFixture : public ::testing::Test
{
public:
    size_t m_capturedSize = 0;
    etl::array<uint8_t, 256> m_capturedBuffer{};

    ::testing::StrictMock<ModbusRtuPortMock> m_portMock;
    ModbusClient<RTUFramer> m_client{m_portMock};

    RtuClientTestFixture()
        : m_client(m_portMock)
    {
        ON_CALL(m_portMock, Transmit(_, _))
            .WillByDefault(DoAll(
                [this](etl::array_view<uint8_t> tx, const auto &)
                {
                    m_capturedSize = tx.size();
                    std::copy(tx.begin(), tx.end(), m_capturedBuffer.begin());
                },
                Return(true)));
    }
    void VerifyTransmitData(const uint8_t *expectedFrame)
    {
        for (size_t i = 0; i < m_capturedSize; ++i)
        {
            // Print each byte as two-digit hex with leading 0
            std::cout << std::hex << std::setfill('0')
                      << std::setw(2) << static_cast<int>(m_capturedBuffer[i]) << " ";
        }
        std::cout << std::dec << std::endl; // back to decimal

        for (size_t i = 0; i < m_capturedSize; ++i)
        {
            EXPECT_EQ(m_capturedBuffer[i], expectedFrame[i]);
        }
    }
};

// Hardcoded happy flow tests
TEST_F(RtuClientTestFixture, ReadHoldingRegisters_Request1)
{

    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output[2] = {0};
    m_client.ReadHoldingRegisters(0x01, 0x0010, 2, output, {});

    // Full expected frame: UnitID, Function, StartAddr, Quantity, CRC16
    const uint8_t expectedFrame[] = {
        0x01, 0x03, 0x00, 0x10, 0x00, 0x02, 0xC5, 0xCE};
    VerifyTransmitData(expectedFrame);
}

TEST_F(RtuClientTestFixture, ReadHoldingRegisters_Request2)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output[4] = {0};
    m_client.ReadHoldingRegisters(0x02, 0x0020, 4, output, {});

    // Full expected frame including CRC
    const uint8_t expectedFrame[] = {
        0x02, 0x03, 0x00, 0x20, 0x00, 0x04, 0x45, 0xF0};

    VerifyTransmitData(expectedFrame);
}

TEST_F(RtuClientTestFixture, ReadCoils_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output = 0;
    m_client.ReadCoils(0x02, 0x0020, 4, &output, {});

    const uint8_t expectedFrame[] = {
        0x02, 0x01, 0x00, 0x20, 0x00, 0x04, 0x3C, 0x30
    };

    VerifyTransmitData(expectedFrame);
}

TEST_F(RtuClientTestFixture, ReadDiscreteInputs_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output = 0;
    m_client.ReadDiscreteInputs(0x02, 0x0020, 4, &output, {});

    const uint8_t expectedFrame[] = {
        0x02, 0x02, 0x00, 0x20, 0x00, 0x04, 0x78, 0x30
    };

    VerifyTransmitData(expectedFrame);
}

TEST_F(RtuClientTestFixture, ReadInputRegisters_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t output[4] = {0};
    m_client.ReadInputRegisters(0x02, 0x0020, 4, output, {});

    const uint8_t expectedFrame[] = {
        0x02, 0x04, 0x00, 0x20, 0x00, 0x04, 0xF0, 0x30
    };

    VerifyTransmitData(expectedFrame);
}

TEST_F(RtuClientTestFixture, WriteSingleCoil_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    m_client.WriteSingleCoil(0x02, 0x0020, true, {});

    const uint8_t expectedFrame[] = {
        0x02, 0x05, 0x00, 0x20, 0xFF, 0x00, 0x8D, 0xC3
    };

    VerifyTransmitData(expectedFrame);
}

TEST_F(RtuClientTestFixture, WriteSingleRegister_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    m_client.WriteSingleRegister(0x02, 0x0020, 0x1234, {});

    const uint8_t expectedFrame[] = {
        0x02, 0x06, 0x00, 0x20, 0x12, 0x34, 0x85, 0x44
    };

    VerifyTransmitData(expectedFrame);
}

TEST_F(RtuClientTestFixture, WriteMultipleRegisters_Request)
{
    EXPECT_CALL(m_portMock, Transmit(_, _));

    uint16_t values[2] = {0x1234, 0x5678};
    m_client.WriteMultipleRegisters(0x02, 0x0020, 2, values, {});

    const uint8_t expectedFrame[] = {
        0x02, 0x10,
        0x00, 0x20,
        0x00, 0x02,
        0x04,
        0x12, 0x34,
        0x56, 0x78,
        0x85, 0xC7
    };

    VerifyTransmitData(expectedFrame);
}
