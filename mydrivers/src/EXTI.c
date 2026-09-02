#include "EXTI.h"

/**
 * @brief  EXTI_Init for valid port and line number.
 *
 * @param  EXTI_InitStruct = User Config Structure
 *
 * @retval void.
 */

void EXTI_Init(EXTI_InitTypeDef_t *EXTI_InitStruct)
{
	uint32_t tempValue = 0;

	tempValue = (uint32_t)EXTI_BASE_ADDR;

	EXTI->IMR &= ~(0x1u << EXTI_InitStruct->EXTI_Line_Number);
	EXTI->EMR &= ~(0x1u << EXTI_InitStruct->EXTI_Line_Number);

	if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
	{
		tempValue += EXTI_InitStruct->EXTI_Mode;

		*((__IO uint32_t *)tempValue) |= (0x1u << EXTI_InitStruct->EXTI_Line_Number);

		tempValue = (uint32_t)EXTI_BASE_ADDR;

		EXTI->RTSR &= ~(0x1u << EXTI_InitStruct->EXTI_Line_Number);
		EXTI->FTSR &= ~(0x1u << EXTI_InitStruct->EXTI_Line_Number);

		if(EXTI_InitStruct->TriggerSelection == EXTI_Trigger_RF)
		{
			EXTI->RTSR |= (0x1u << EXTI_InitStruct->EXTI_Line_Number);
			EXTI->FTSR |= (0x1u << EXTI_InitStruct->EXTI_Line_Number);
		}
		else
		{
			tempValue += EXTI_InitStruct->TriggerSelection;

			*((__IO uint32_t *)tempValue) |= (0x1u << EXTI_InitStruct->EXTI_Line_Number);
		}
	}
	else
	{
		tempValue = (uint32_t)EXTI_BASE_ADDR;

		tempValue += EXTI_InitStruct->EXTI_Mode;

		*((__IO uint32_t *)tempValue) &= ~(0x1u << EXTI_InitStruct->EXTI_Line_Number);
	}
}

/**
 * @brief  EXTI_Lineconfig, Configures the port and pin for SYSCFG.
 *
 * @param  PortSource = Port Value A - I @def_group PORT_Values
 *
 * @param  EXTI_LineConfig = Pin Numbers & Line Numbers @def_group EXTI_Line_Values.
 *
 * @retval void.
 */

void EXTI_LineConfig(uint8_t PortSource, uint8_t EXTI_LineSource)
{
	uint32_t tempValue;

	tempValue = SYSCFG->EXTI_CR[EXTI_LineSource >> 2U];
	tempValue &= ~(0xFU << (EXTI_LineSource & 0x3U) * 4);
	tempValue = (PortSource << (EXTI_LineSource & 0x3U) * 4);
	SYSCFG->EXTI_CR[EXTI_LineSource >> 2U] = tempValue;
}

/**
 * @brief  NVIC_EnableInterrupt.
 *
 * @param  IRQNumber = IRQ Number of Line
 *
 * @retval void.
 */

void NVIC_EnableInterrupt(IRQNumber_Typedef_t IRQNumber)
{
	*((IRQNumber >> 5U) + NVIC_ISER0) = (0x1U << (IRQNumber % 32));
}
