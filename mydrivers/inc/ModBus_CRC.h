#ifndef INC_MODBUS_CRC_H_
#define INC_MODBUS_CRC_H_

#include <stm32f407xx.h>

#define MODBUS_CRC_POLYNOMIAL		0xA001U
#define MODBUS_CRC_INITIAL		0xFFFFU

void Modbus_CRC_GenerateTable(void);

uint16_t Modbus_CalculateCRC(const uint8_t *data, uint16_t length);

#endif
