#include "Modbus.h"
#include "Modbus_CRC.h"

static uint16_t Modbus_GetU16(const uint8_t *data)
{
    return ((uint16_t)data[0] << 8U) |
           ((uint16_t)data[1]);
}

Modbus_FrameStatus_t Modbus_ValidateFrame(const uint8_t *rxBuffer, uint16_t rxLength)
{
    uint16_t receivedCRC;
    uint16_t calculatedCRC;

    uint16_t startingAddress;
    uint16_t quantity;

    /*
     * 1. Minimum length
     */
    if (rxLength < MODBUS_MIN_FRAME_SIZE)
    {
        return MODBUS_FRAME_INCOMPLETE;
    }

    /*
     * 2. Slave address
     */
    if (rxBuffer[0] != MODBUS_SLAVE_ADDRESS &&
        rxBuffer[0] != MODBUS_BROADCAST_ADDRESS)
    {
        return MODBUS_FRAME_NOT_FOR_US;
    }

    /*
     * 3. CRC
     */
    receivedCRC =
        ((uint16_t)rxBuffer[rxLength - 1U] << 8U) |
         (uint16_t)rxBuffer[rxLength - 2U];

    calculatedCRC = Modbus_CalculateCRC(rxBuffer, rxLength - MODBUS_CRC_SIZE);

    if (receivedCRC != calculatedCRC)
    {
        return MODBUS_FRAME_INVALID_CRC;
    }

    /*
     * 4. Function code
     */
    switch (rxBuffer[1])
    {
        case MODBUS_FC_READ_HOLDING_REGISTERS:
            break;

        default:
            return MODBUS_FRAME_UNSUPPORTED_FUNCTION;
    }

    /*
     * 5. Function-specific validation
     */
    if (rxBuffer[1] == MODBUS_FC_READ_HOLDING_REGISTERS)
    {
        /*
         * 0x03 request must be exactly 8 bytes.
         */
        if (rxLength != MODBUS_READ_REQUEST_SIZE)
        {
            return MODBUS_FRAME_INVALID_DATA_LENGTH;
        }

        /*
         * Extract request fields.
         */
        startingAddress = Modbus_GetU16(&rxBuffer[2]);
        quantity = Modbus_GetU16(&rxBuffer[4]);

        /*
         * Quantity cannot be zero.
         */
        if (quantity == 0U)
        {
            return MODBUS_FRAME_INVALID_QUANTITY;
        }

        /*
         * First requested register
         * must exist.
         */
        if (startingAddress >= MODBUS_HOLDING_REG_COUNT)
        {
            return MODBUS_FRAME_INVALID_REGISTER_ADDRESS;
        }

        /*
         * Entire requested range
         * must exist.
         */
        if (quantity > (MODBUS_HOLDING_REG_COUNT - startingAddress))
        {
            return MODBUS_FRAME_INVALID_REGISTER_ADDRESS;
        }
    }

    /*
     * 6. Broadcast
     */
    if (rxBuffer[0] == MODBUS_BROADCAST_ADDRESS)
    {
        return MODBUS_FRAME_BROADCAST;
    }

    return MODBUS_FRAME_OK;
}

uint16_t Modbus_BuildExceptionResponse(uint8_t slaveAddress, uint8_t functionCode,
										Modbus_ExceptionCode_t exceptionCode, uint8_t *txBuffer)
{
    uint16_t crc;

    txBuffer[0] = slaveAddress;

    txBuffer[1] = functionCode | 0x80U;

    txBuffer[2] = (uint8_t)exceptionCode;

    crc = Modbus_CalculateCRC(txBuffer, 3U);

    txBuffer[3] = (uint8_t)(crc & 0x00FFU);

    txBuffer[4] = (uint8_t)((crc >> 8U) & 0x00FFU);

    return 5U;
}

void Modbus_ParseFrame(uint8_t *rxBuffer, uint16_t rxLength, ModbusFrame_t *frame)
{
    frame->slaveAddress = rxBuffer[0];

    frame->functionCode = rxBuffer[1];

    frame->data = &rxBuffer[2];

    frame->dataLength = rxLength - MODBUS_HEADER_SIZE - MODBUS_CRC_SIZE;

    frame->crc = ((uint16_t)rxBuffer[rxLength - 1U] << 8U) |
    				(uint16_t)rxBuffer[rxLength - 2U];
}
