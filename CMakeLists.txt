cmake_minimum_required(VERSION 3.20)
project(FokkerModbusLib VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Options ----
option(BUILD_TESTS "Build unit tests" ON)

# ---- ETL ----
add_subdirectory(../etl etl_build EXCLUDE_FROM_ALL)

# ---- Library ----
add_library(FokkerModbusRtuClient
    RTU/modbus_RTU_client.cpp
)


target_include_directories(FokkerModbusRtuClient
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/general
        ${CMAKE_CURRENT_SOURCE_DIR}/RTU
)

target_link_libraries(FokkerModbusRtuClient
    PUBLIC 
        etl
)

target_compile_definitions(FokkerModbusRtuClient
    PUBLIC ETL_ENDIAN_NATIVE=0
)

# ---- Tests ----
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(gtest)
endif()
