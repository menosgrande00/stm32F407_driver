#include "Modbus_CRC.h"

static uint16_t crc_table[256];


void Modbus_CRC_GenerateTable(void)
{
    for (uint16_t i = 0; i < 256U; i++)
    {
        uint16_t crc = i;

        for (uint8_t bit = 0; bit < 8U; bit++)
        {
            if (crc & 0x0001U)
            {
                crc = (crc >> 1U) ^ MODBUS_CRC_POLYNOMIAL;
            }
            else
            {
                crc >>= 1U;
            }
        }

        crc_table[i] = crc;
    }
}


uint16_t Modbus_CalculateCRC(const uint8_t *data, uint16_t length)
{
    uint16_t crc = MODBUS_CRC_INITIAL;

    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t tableIndex = (uint8_t)((crc ^ data[i]) & 0x00FFU);

        crc = (crc >> 8U) ^ crc_table[tableIndex];
    }
    return crc;
}
