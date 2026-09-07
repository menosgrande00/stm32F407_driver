#include "USART.h"

/**
 * @brief  USART Init, Configures the USART Peripheral.
 *
 * @param  USART_Handle = User Config Structure.
 *
 * @retval void.
 */
void USART_Init(USART_HandleTypeDef_t *USART_Handle)
{
	uint32_t periphClock = 0;
	uint32_t mantissaPart = 0;
	uint32_t fractionPart = 0;
	uint32_t USART_DIV_Value = 0;
	uint32_t tempValue = 0;

	uint32_t tempReg = 0;

	tempReg = USART_Handle->Instance->CR1;

	tempReg |= (USART_Handle->Init.Mode)
			|  (USART_Handle->Init.OverSampling)
	        |  (USART_Handle->Init.WordLength)
			|  (USART_Handle->Init.Parity);
	USART_Handle->Instance->CR1 = tempReg;

/******** 		StopBits		***********/

	tempReg = USART_Handle->Instance->CR2;

	tempReg &= ~(0x3U << UART_CR2_STOP);
	tempReg |= (USART_Handle->Init.StopBits);
	USART_Handle->Instance->CR2 = tempReg;

/******** 		HardWareFlowControl		***********/

	tempReg = USART_Handle->Instance->CR3;
	tempReg |= (USART_Handle->Init.HardwareFlowControl);
	USART_Handle->Instance->CR3 = tempReg;

/******** 		Baud Rate Configuration		***********/


	if (USART_Handle->Instance == USART1 || USART_Handle->Instance == USART6)
	{
		periphClock = RCC_GetPClock2();
	}
	else
	{
		periphClock = RCC_GetPClock1();
	}
	if (USART_Handle->Init.OverSampling == USART_OVERSAMPLE_8)
	{
		USART_DIV_Value = __USART_DIV_VALUE_8(periphClock, USART_Handle->Init.BaudRate);
		mantissaPart = (USART_DIV_Value / 100U);
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100);

		fractionPart = ( ( (fractionPart * 8U) + 50U) / 100U ) & (0x07U);
	}
	else
	{
		USART_DIV_Value = __USART_DIV_VALUE_16(periphClock, USART_Handle->Init.BaudRate);
		mantissaPart = (USART_DIV_Value / 100U);
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100);

		fractionPart = ( ( (fractionPart * 16U) + 50U) / 100U ) & (0x0FU);

	}
	tempValue |= (mantissaPart << 4U);
	tempValue |= (fractionPart);

	USART_Handle->Instance->BRR = tempValue;
}



/**
 * @brief  USART_TransmitData, Transmits data through USART.
 *
 * @param  USART_Handle = User Config Structure.
 *
 * @param  pData = Address of data to send.
 *
 * @param  dataSize = Length of your data in bytes.
 *
 * @retval void.
 */
void USART_TransmitData(USART_HandleTypeDef_t *USART_Handle, uint8_t *pData, uint16_t dataSize)
{
	uint16_t *data16Bits;

	if (USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits
			&& USART_Handle->Init.Parity == USART_PARITY_NONE)
	{
		data16Bits = (uint16_t *)pData;
	}
	else
	{
		data16Bits = NULL;
	}
	while (dataSize > 0)
	{
		while( !(USART_GetFlagStatus(USART_Handle, USART_TxE_FLAG)) );

		if (data16Bits == NULL)
		{
			USART_Handle->Instance->DR = (uint8_t)(*pData & (0xFFU));
			pData++;
			dataSize--;
		}
		else
		{
			USART_Handle->Instance->DR = ((uint16_t)(*data16Bits & (0x01FF)));
			data16Bits++;
			dataSize -= 2;
		}
	}
	while (!(USART_GetFlagStatus(USART_Handle, USART_TC_FLAG)));

}

/**
 * @brief  USART_ReceiveData, Receives data through USART.
 *
 * @param  USART_Handle = User Config Structure.
 *
 * @param  pBuffer = Address of buffer where received data will be stored.
 *
 * @param  dataSize = Length of data to receive in bytes.
 *
 * @retval void.
 */

void USART_ReceiveData(USART_HandleTypeDef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize)
{
	uint16_t *p16BitsBuffer;
	uint8_t *p8BitsBuffer;

	if (USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits
			&& USART_Handle->Init.Parity == USART_PARITY_NONE)
	{
		p16BitsBuffer = (uint16_t *)pBuffer;
		p8BitsBuffer = NULL;
	}
	else
	{
		p8BitsBuffer = (uint8_t *)pBuffer;
		p16BitsBuffer = NULL;
	}
	while(dataSize > 0)
	{
		while(!(USART_GetFlagStatus(USART_Handle, USART_RxNE_FLAG) ) );
		if(p8BitsBuffer == NULL)
		{
			*p16BitsBuffer = (uint16_t)(USART_Handle->Instance->DR & 0x01FF);
			p16BitsBuffer++;
			dataSize -= 2;
		}
		else
		{
			if (USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bits
					&& USART_Handle->Init.Parity != USART_PARITY_NONE)
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
			}
			else if (USART_Handle->Init.WordLength == USART_WORDLENGTH_8Bits
					&& USART_Handle->Init.Parity == USART_PARITY_NONE)
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
			}
			else
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x007FU);
			}
			p8BitsBuffer++;
			dataSize--;
		}
	}
}


/**
 * @brief  USART_PeriphCmd, Enable or Disable USART Peripheral.
 *
 * @param  USART_Handle = User Config Structure.
 *
 * @param  stateOfUSART = ENABLE or DISABLE.
 *
 * @retval void.
 */
void USART_PeriphCmd(USART_HandleTypeDef_t *USART_Handle, FunctionalState_t stateOfUSART)
{
	if (stateOfUSART == ENABLE)
	{
		USART_Handle->Instance->CR1 |= (0x1U << USART_CR1_UE);
	}
	else
	{
		USART_Handle->Instance->CR1 &= ~(0x1U << USART_CR1_UE);
	}
}



/**
 * @brief  USART_GetFlagStatus, Returns the status of the specified USART flag.
 *
 * @param  USART_Handle = User Config Structure.
 *
 * @param  flagName = Flag name of SR register.
 *
 * @retval USART_FlagStatus_t.
 */
USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypeDef_t *USART_Handle, uint16_t flagName)
{
	return ((USART_Handle->Instance->SR & flagName) ? USART_FLAG_SET : USART_FLAG_RESET);
}
