#include "I2C.h"

void I2C_Init(I2C_HandleTypeDef_t *I2C_Handle)
{
	uint32_t pClockValue = 0x0U;

	pClockValue = RCC_GetPClock1();

	if (__Check_PClockValue(pClockValue, I2C_Handle->Init.ClockSpeed) != 1)
	{
		uint32_t tempReg = 0x0U;
		uint32_t freqValue = 0x0U;

		/************	ACK_State & Stretching Mode Config	************/

		tempReg = I2C_Handle->Instance->CR1;

		tempReg |= ((I2C_Handle->Init.ACK_State)
				|  (I2C_Handle->Init.ClockStretch));

		I2C_Handle->Instance->CR1 = tempReg;

		/************	Frequency Value of PClock for Clock Generation	************/

		freqValue = I2C_Get_FreqValue(pClockValue);

		tempReg = I2C_Handle->Instance->CR2;
		tempReg |= (freqValue << 0U);
		I2C_Handle->Instance->CR2 = tempReg;

		/************	Address Configuration	************/

		tempReg = I2C_Handle->Instance->OAR1;

		tempReg |= (I2C_Handle->Init.AddressingMode);

		if (I2C_Handle->Init.AddressingMode == I2C_ADDR_MODE_7)
		{
			tempReg |= (I2C_Handle->Init.MyOwnAddress << 1U);
		}
		else
		{
			tempReg |= (I2C_Handle->Init.MyOwnAddress << 0U);
		}
	}
}

/**
 * @brief  I2C_PerphCmd, Enable or Disable I2C Peripheral.
 *
 * @param  I2Cx = I2C Peripheral.
 *
 * @param  stateOfI2C = ENABLE or DISABLE.
 *
 * @retval void.
 */

void I2C_PeriphCmd(I2C_TypeDef_t *I2Cx, FunctionalState_t stateOfI2C)
{
	if (stateOfI2C == ENABLE)
	{
		I2Cx->CR1 |= (0x1U << I2C_CR1_PE);
	}
	else
	{
		I2Cx->CR1 &= ~(0x1U << I2C_CR1_PE);
	}
}
