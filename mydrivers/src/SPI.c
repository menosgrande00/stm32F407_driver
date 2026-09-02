#include "SPI.h"

/**
 * @brief  SPI_CloseISR_TX, Disables the interrupt for Transmission.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

static void SPI_CloseISR_TX(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->CR2 &= ~(0x1U << SPI_CR2_TXEIE);
	SPI_Handle->TxDataSize = 0;
	SPI_Handle->pTxDataAddr = NULL;
	SPI_Handle->busStateTx = SPI_BUS_FREE;
}



/**
 * @brief  SPI_TransmitHelper_16Bits, Transmits 16-bit data during interrupt.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

static void SPI_TransmitHelper_16Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->DR = *((uint16_t *)(SPI_Handle->pTxDataAddr));
	SPI_Handle->pTxDataAddr += sizeof(uint16_t);
	SPI_Handle->TxDataSize -= 2;

	if (SPI_Handle->TxDataSize == 0)
	{
		SPI_CloseISR_TX(SPI_Handle);
	}
}



/**
 * @brief  SPI_TransmitHelper_8Bits, Transmits 8-bit data during interrupt.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

static void SPI_TransmitHelper_8Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->DR = *((uint8_t *)(SPI_Handle->pTxDataAddr));
	SPI_Handle->pTxDataAddr += sizeof(uint8_t);
	SPI_Handle->TxDataSize--;

	if (SPI_Handle->TxDataSize == 0)
	{
		SPI_CloseISR_TX(SPI_Handle);
	}
}



/**
 * @brief  SPI_CloseISR_RX, Disables the interrupt for Reception.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

static void SPI_CloseISR_RX(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->CR2 &= ~(0x1U << SPI_CR2_RXNEIE);
	SPI_Handle->RxDataSize = 0;
	SPI_Handle->pRxDataAddr = NULL;
	SPI_Handle->busStateRx = SPI_BUS_FREE;
}



/**
 * @brief  SPI_ReceiveHelper_16Bits, Receives 16-bit data during interrupt.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

static void SPI_ReceiveHelper_16Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
    *((uint16_t *)SPI_Handle->pRxDataAddr) =
        *((__IO uint16_t *)(&SPI_Handle->Instance->DR));
	SPI_Handle->pRxDataAddr += sizeof(uint16_t);
	SPI_Handle->RxDataSize -= 2;

    if (SPI_Handle->RxDataSize == 0)
    {
        SPI_CloseISR_RX(SPI_Handle);
    }
}



/**
 * @brief  SPI_ReceiveHelper_8Bits, Receives 8-bit data during interrupt.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

static void SPI_ReceiveHelper_8Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	*((uint8_t *)SPI_Handle->pRxDataAddr) = *((__IO uint8_t *)(&SPI_Handle->Instance->DR));
	SPI_Handle->pRxDataAddr += sizeof(uint8_t);
	SPI_Handle->RxDataSize--;

	if (SPI_Handle->RxDataSize == 0)
	{
		SPI_CloseISR_RX(SPI_Handle);
	}
}



/**
 * @brief  SPI Init, Configures the SPI Peripheral.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

void SPI_Init(SPI_HandleTypeDef_t *SPI_Handle)
{
	uint32_t tempValue = 0;

	tempValue = SPI_Handle->Instance->CR1;

	tempValue |= (SPI_Handle->Init.BaudRate)
	          |  (SPI_Handle->Init.CPHA)
	          |  (SPI_Handle->Init.CPOL)
	          |  (SPI_Handle->Init.DFF_Format)
	          |  (SPI_Handle->Init.Mode)
	          |  (SPI_Handle->Init.FrameFormat)
			  |  (SPI_Handle->Init.Bus_Config)
			  |  (SPI_Handle->Init.SSM_Cmd);
	SPI_Handle->Instance->CR1 = tempValue;
}



/**
 * @brief  SPI_PeriphCmd, Enable or Disable SPI SPE Peripheral.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @param  StateofSPI = ENABLE or DISABLE.
 *
 * @retval void.
 */

void SPI_PeriphCmd(SPI_HandleTypeDef_t *SPI_Handle, FunctionalState_t StateofSPI)
{
	if (StateofSPI == ENABLE)
	{
		SPI_Handle->Instance->CR1 |= (0x1U << SPI_CR1_SPE);
	}
	else
	{
		SPI_Handle->Instance->CR1 &= ~(0x1U << SPI_CR1_SPE);
	}
}



/**
 * @brief  SPI_TransmitData, Transmits data to the slave.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @param  pData = Address of data to send.
 *
 * @param  sizeOfData = Length of your data in bytes.
 *
 * @retval void.
 */

void SPI_TransmitData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pData, uint32_t sizeOfData)
{
	if (SPI_Handle->Init.DFF_Format == SPI_DFF_16BITS)
	{
		while (sizeOfData > 0 )
		{
			if (SPI_GetFlagStatus(SPI_Handle, SPI_TxE_Flag))
			{
				SPI_Handle->Instance->DR = *( (uint16_t *)pData );
				pData += sizeof(uint16_t);
				sizeOfData -= 2;
			}
		}
	}
	else
	{
		while (sizeOfData > 0 )
		{
			if (SPI_GetFlagStatus(SPI_Handle, SPI_TxE_Flag))
			{
				SPI_Handle->Instance->DR = *pData;
				pData++;
				sizeOfData--;
			}
		}
	}

	while (SPI_GetFlagStatus(SPI_Handle, SPI_BSY_Flag)); // Wait for Busy Flag
}



/**
 * @brief  SPI_TransmitData_IT, Transmits data to the slave with interrupt.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @param  pData = Address of data to send.
 *
 * @param  sizeOfData = Length of your data in bytes.
 *
 * @retval void.
 */

void SPI_TransmitData_IT(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pData, uint32_t sizeOfData)
{

	SPI_Bus_Status_t busState = SPI_Handle->busStateTx;

	if(busState != SPI_BUS_BUSY_TX)
	{
		SPI_Handle->pTxDataAddr = (uint8_t *)pData;
		SPI_Handle->TxDataSize = sizeOfData;
		SPI_Handle->busStateTx = SPI_BUS_BUSY_TX;

		if (SPI_Handle->Instance->CR1 & (0x1U << SPI_CR1_DFF))
		{
			SPI_Handle->TxISRFunction = SPI_TransmitHelper_16Bits;
		}
		else
		{
			SPI_Handle->TxISRFunction = SPI_TransmitHelper_8Bits;
		}

		SPI_Handle->Instance->CR2 |= (0x1U << SPI_CR2_TXEIE);
	}
}



/**
 * @brief  SPI_ReceiveData_IT, Receives data from the slave with interrupt.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @param  pBuffer = Address of data to store.
 *
 * @param  sizeOfData = Length of your data in bytes.
 *
 * @retval void.
 */

void SPI_ReceiveData_IT(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pBuffer, uint32_t sizeOfData)
{
	SPI_Bus_Status_t busState = SPI_Handle->busStateRx;

	if(busState != SPI_BUS_BUSY_RX)
	{
		SPI_Handle->pRxDataAddr = (uint8_t *)pBuffer;
		SPI_Handle->RxDataSize = sizeOfData;
		SPI_Handle->busStateRx = SPI_BUS_BUSY_RX;

		if (SPI_Handle->Instance->CR1 & (0x1U << SPI_CR1_DFF))
		{
			SPI_Handle->RxISRFunction = SPI_ReceiveHelper_16Bits;
		}
		else
		{
			SPI_Handle->RxISRFunction = SPI_ReceiveHelper_8Bits;
		}

		SPI_Handle->Instance->CR2 |= (0x1U << SPI_CR2_RXNEIE);
	}
}



/**
 * @brief  SPI_TransmitData, Receive data from the slave.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @param  pBuffer = Address of data to store.
 *
 * @param  sizeOfData = Length of your data in bytes.
 *
 * @retval void.
 */

void SPI_ReceiveData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pBuffer, uint32_t sizeOfData)
{
	if (SPI_Handle->Init.DFF_Format == SPI_DFF_16BITS)
	{
		while (sizeOfData > 0 )
		{
			if (SPI_GetFlagStatus(SPI_Handle, SPI_RxNE_Flag))
			{
				*((uint16_t *)pBuffer) = (uint16_t)SPI_Handle->Instance->DR;
				pBuffer += sizeof(uint16_t);
				sizeOfData -= 2;
			}
		}
	}
	else
	{
		while (sizeOfData > 0 )
		{
			if (SPI_GetFlagStatus(SPI_Handle, SPI_RxNE_Flag))
			{
				*pBuffer = *((__IO uint8_t *)&SPI_Handle->Instance->DR);
				pBuffer++;
				sizeOfData--;
			}
		}
	}
}



/**
 * @brief  SPI_InterruptHandler, Handles SPI transmit and receive interrupts.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @retval void.
 */

void SPI_InterruptHandler(SPI_HandleTypeDef_t *SPI_Handle)
{
	uint8_t interruptSource = 0;
	uint8_t interruptFlag = 0;

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_TXEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_TxE);

	if ( (interruptSource != 0) && (interruptFlag != 0) )
	{
		SPI_Handle->TxISRFunction(SPI_Handle);
	}

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_RXNEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_RXNE);

	if ( (interruptSource != 0) && (interruptFlag != 0) )
	{
		SPI_Handle->RxISRFunction(SPI_Handle);
	}
}



/**
 * @brief  SPI_GetFlagStatus, Return the flag of SR register.
 *
 * @param  SPI_Handle = User Config Structure.
 *
 * @param  SPI_flag = flag name of SR register.
 *
 * @retval SPI_FlagStatus_t.
 */

SPI_FlagStatus_t SPI_GetFlagStatus(SPI_HandleTypeDef_t *SPI_Handle, uint16_t SPI_flag)
{
	return ((SPI_Handle->Instance->SR & SPI_flag) ? SPI_FLAG_SET : SPI_FLAG_RESET);
}
