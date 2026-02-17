# Fetch GoogleTest automatically
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(FokkerModbusLibTests
    RTU/modbus_RTU_client_tests.cpp
)

target_link_libraries(FokkerModbusLibTests
    PRIVATE FokkerModbusRtuClient
    PRIVATE GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FokkerModbusLibTests)
