
    bool ModbusPort::Receive(etl::array_view<BufferDataType> transmitBuffer, uint16_t timeoutMs, const ReceiveCallback &receiveCallback){
        //DOSHIT
        (void) transmitBuffer; 
        (void) timeoutMs;
        (void) receiveCallback;
        return false;
    }
    bool ModbusPort::Transmit(etl::array_view<BufferDataType> transmitBuffer, const TransmitCallback &transmitCallback){
        //DOSHIT
        (void) transmitBuffer; 
        (void) transmitCallback;
        return false;
    }
