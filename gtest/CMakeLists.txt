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

target_include_directories(FokkerModbusLibTests
     PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/gmock)

target_link_libraries(FokkerModbusLibTests
    PRIVATE FokkerModbusLib
    PRIVATE GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(FokkerModbusLibTests)
