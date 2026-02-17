
#ifndef FOKKER_MODBUS_CLIENT_INTERFACE_H_
#define FOKKER_MODBUS_CLIENT_INTERFACE_H_

#include "general_modbus_definitions.h"

class ModbusClientInterface
{
public:
    enum class Result 
    {
        NO_ERROR = 0,
        SHIT_HAPPEND = 1,
    };
    using DoneCallback = etl::delegate<void(Result)>;

    virtual void Start() = 0;

    // --- Read Functions ---
    // virtual void ReadCoils(uint8_t targetAddress,
    //                        uint16_t registerAdress,
    //                        uint16_t quantity,
    //                        uint8_t *outputBuffer,
    //                        const DoneCallback &callback) = 0;

    // virtual void ReadDiscreteInputs(uint8_t targetAddress,
    //                                 uint16_t registerAdress,
    //                                 uint16_t quantity,
    //                                 uint8_t *outputBuffer,
    //                                 const DoneCallback &callback) = 0;

    virtual void ReadHoldingRegisters(uint8_t targetAddress,
                                      uint16_t registerAdress,
                                      uint16_t quantity,
                                      uint16_t *outputBuffer,
                                      const DoneCallback &callback) = 0;

    virtual void ReadInputRegisters(uint8_t targetAddress,
                                    uint16_t registerAdress,
                                    uint16_t quantity,
                                    uint16_t *outputBuffer,
                                    const DoneCallback &callback) = 0;

    // --- Write Functions (Single) ---
    virtual void WriteSingleCoil(uint8_t targetAddress,
                                 uint16_t registerAdress,
                                 bool value,
                                 const DoneCallback &callback) = 0;

    virtual void WriteSingleRegister(uint8_t targetAddress,
                                     uint16_t registerAdress,
                                     uint16_t value,
                                     const DoneCallback &callback) = 0;

    // --- Write Functions (Multiple) ---
    // virtual void WriteMultipleCoils(uint8_t targetAddress,
    //                                 uint16_t registerAdress,
    //                                 const uint8_t *values,
    //                                 uint16_t quantity,
    //                                 const DoneCallback &callback) = 0;

    virtual void WriteMultipleRegisters(uint8_t targetAddress,
                                        uint16_t registerAdress,
                                        const uint16_t *values,
                                        uint16_t quantity,
                                        const DoneCallback &callback) = 0;

    // --- Optional Diagnostics ---
    virtual void Diagnostics(uint8_t targetAddress,
                             uint16_t subFunction,
                             uint16_t data,
                             const DoneCallback &callback) = 0;

    // --- Control ---
    // virtual bool IsBusy() const = 0;
    // virtual void Cancel() = 0;
};

#endif // FOKKER_MODBUS_CLIENT_INTERFACE_H_