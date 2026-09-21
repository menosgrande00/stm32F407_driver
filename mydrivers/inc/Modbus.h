#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_

#include <stm32f407xx.h>

#define MODBUS_SLAVE_ADDRESS          0x01U
#define MODBUS_BROADCAST_ADDRESS      0x00U

#define MODBUS_MIN_FRAME_SIZE         4U
#define MODBUS_READ_REQUEST_SIZE      8U

#define MODBUS_HEADER_SIZE            2U
#define MODBUS_CRC_SIZE               2U


#define MODBUS_HOLDING_REG_START     0x0000U
#define MODBUS_HOLDING_REG_COUNT     5U


typedef enum
{
    MODBUS_FC_READ_COILS               = 0x01U,
    MODBUS_FC_READ_DISCRETE_INPUTS     = 0x02U,
    MODBUS_FC_READ_HOLDING_REGISTERS   = 0x03U,
    MODBUS_FC_READ_INPUT_REGISTERS     = 0x04U,

    MODBUS_FC_WRITE_SINGLE_COIL        = 0x05U,
    MODBUS_FC_WRITE_SINGLE_REGISTER    = 0x06U,

    MODBUS_FC_WRITE_MULTIPLE_COILS     = 0x0FU,
    MODBUS_FC_WRITE_MULTIPLE_REGISTERS = 0x10U

} Modbus_FunctionCode_t;


typedef enum
{
    MODBUS_FRAME_OK = 0,

    MODBUS_FRAME_INCOMPLETE,

    MODBUS_FRAME_NOT_FOR_US,

    MODBUS_FRAME_BROADCAST,

    MODBUS_FRAME_INVALID_CRC,

    MODBUS_FRAME_UNSUPPORTED_FUNCTION,

    MODBUS_FRAME_INVALID_DATA_LENGTH,

    MODBUS_FRAME_INVALID_REGISTER_ADDRESS,

    MODBUS_FRAME_INVALID_QUANTITY

} Modbus_FrameStatus_t;


typedef enum
{
    MODBUS_EXCEPTION_ILLEGAL_FUNCTION      = 0x01U,

    MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS  = 0x02U,

    MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE    = 0x03U

} Modbus_ExceptionCode_t;


typedef struct
{
    uint8_t slaveAddress;

    uint8_t functionCode;

    uint8_t *data;

    uint16_t dataLength;

    uint16_t crc;

} ModbusFrame_t;

Modbus_FrameStatus_t Modbus_ValidateFrame(const uint8_t *rxBuffer, uint16_t rxLength);
void Modbus_ParseFrame(uint8_t *rxBuffer, uint16_t rxLength, ModbusFrame_t *frame);
uint16_t Modbus_BuildExceptionResponse(uint8_t slaveAddress, uint8_t functionCode,
										Modbus_ExceptionCode_t exceptionCode, uint8_t *txBuffer);


//typedef struct
//{
//	ModbusFrame_t msg;
//	u8 valid;
//}msg;
//
//typedef struct
//{
//	msg messages[20];
//	u8 buffer;
//	u16 ct;
//}process_msg;


#endif
