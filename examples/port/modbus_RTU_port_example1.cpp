
    bool ModbusPort::Receive(etl::array_view<uint8_t> transmitBuffer, uint16_t timeoutMs, const ModbusPortInterface::ReceiveCallback &receiveCallback){
        //DOSHIT
        (void) transmitBuffer; 
        (void) timeoutMs;
        (void) receiveCallback;
        return false;
    }
    bool ModbusPort::Transmit(etl::array_view<uint8_t> transmitBuffer, const ModbusPortInterface::TransmitCallback &transmitCallback){
        //DOSHIT
        (void) transmitBuffer; 
        (void) transmitCallback;
        return false;
    }
