#include "modbus_client.h"
#include "RTU_framer.h"
#include "modbus_port_mock.h"

#include <gtest/gtest.h>

class RtuClientTestFixture
{
    NiceMock<ModbusRtuPortMock> m_portMock;
    ModbusClient<RTUFramer> m_client {m_portMock};
    // ModbusRtuClient m_client;
};

TEST(RtuClientTestFixture, BasicTest)
{
    EXPECT_EQ(1 + 1, 2);
}
