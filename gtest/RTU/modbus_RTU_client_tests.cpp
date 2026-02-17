#include "modbus_RTU_client.h"

#include <gtest/gtest.h>

class RtuClientTestFixture
{
    ModbusRtuClient m_client;
};

TEST(RtuClientTestFixture, BasicTest)
{
    EXPECT_EQ(1 + 1, 2);
}
