#include "GPIO.h"

/**
 * @brief  GPIO Init, Configures the port and pin.
 *
 * @param  GPIOx pointer to the GPIO peripheral whose pin will be read.
 *
 * @param  GPIO_InitTypeDef_t = User Config Structures.
 *
 * @retval void.
 */

void			GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)
{
	uint32_t	pos;
	uint32_t	fakepos = 0;
	uint32_t	lastpos = 0;

	for(pos = 0; pos < 16; pos++)
	{
		fakepos = (0x1 << pos);
		lastpos = (GPIO_ConfigStruct->pinNumber) & fakepos;

		if (fakepos == lastpos)
		{

			/* MODE CONFIG */
			uint32_t tempValue = GPIOx->MODER;

			tempValue &= ~(0x3u << (pos * 2));
			tempValue |= (GPIO_ConfigStruct->Mode << (pos * 2));

			GPIOx->MODER = tempValue;

			if (GPIO_ConfigStruct->Mode == GPIO_MODE_INPUT || GPIO_ConfigStruct->Mode == GPIO_MODE_ANALOG)
			{
				/* OTYPE CONFIG*/

				tempValue = GPIOx->OTYPER;

				tempValue &= ~(0x1u << (pos));
				tempValue |= (GPIO_ConfigStruct->Otype << (pos));

				GPIOx->OTYPER = tempValue;

				/* OSpeed CONFIG*/

				tempValue = GPIOx->OSPEEDR;

				tempValue &= ~(0x3u << (pos * 2));
				tempValue |= (GPIO_ConfigStruct->Speed << (pos * 2));

				GPIOx->OSPEEDR = tempValue;
			}

			/* PUPDR CONFIG*/

			tempValue = GPIOx->PUPDR;

			tempValue &= ~(0x3u << (pos * 2));
			tempValue |= (GPIO_ConfigStruct->PuPd << (pos * 2));

			GPIOx->PUPDR = tempValue;

			/* Alternate CONFIG*/

			if(GPIO_ConfigStruct->Mode == GPIO_MODE_AF)
			{
				tempValue = GPIOx->AFR[pos >> 3U];
				tempValue &= ~(0xFu << ((pos & 0x7U) * 4));
				tempValue |= (GPIO_ConfigStruct->Alternate << ((pos & 0x7U) * 4));
				GPIOx->AFR[pos >> 3U] = tempValue;
			}
		}
	}
}

/**
 * @brief  Writes a value to the specified GPIO output pin.
 *
 * @param  GPIOx pointer to the GPIO peripheral whose pin will be controlled.
 *
 * @param  pinNumber specifies the GPIO pin to be written 0-15.
 *
 * @param  pinState specifies the state to be written to the pin.
 *         This parameter can be GPIO_Pin_Set or GPIO_Pin_Reset.
 *
 * @retval None
 */

void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState)
{
	if (pinState == GPIO_Pin_Set)
	{
		GPIOx->BSRR = pinNumber;
	}
	else
	{
		GPIOx->BSRR = (pinNumber << 16u);
	}
}

/**
 * @brief  Reads the state of the specified GPIO input pin.
 *
 * @param  GPIOx pointer to the GPIO peripheral whose pin will be read.
 *
 * @param  pinNumber specifies the GPIO pin to be read.
 *
 * @retval GPIO_Pin_Set if the pin is high, GPIO_Pin_Reset if the pin is low.
 */

GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	GPIO_PinState_t bitStatus = GPIO_Pin_Reset;

	if ((GPIOx->IDR & pinNumber) != GPIO_Pin_Reset)
	{
		bitStatus = GPIO_Pin_Set;
	}

	return bitStatus;
}

/**
 * @brief  GPIO_Lock_pin, locks the pin of GPIOx Port.
 *
 * @param  GPIOx pointer to the GPIO peripheral whose pin will be read.
 *
 * @param  pinNumber specifies the GPIO pin to be read.
 *
 * @retval void.
 */


void			GPIO_LockPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	uint32_t tempValue = (0x1u << 16u) | pinNumber;

	GPIOx->LCKR = tempValue;
	GPIOx->LCKR = pinNumber;
	GPIOx->LCKR = tempValue;
	tempValue = GPIOx->LCKR;
}

/**
 * @brief  GPIO_Toggle_pin, toggles the pin of GPIOx Port.
 *
 * @param  GPIOx pointer to the GPIO peripheral whose pin will be read.
 *
 * @param  pinNumber specifies the GPIO pin to be read.
 *
 * @retval void.
 */

void			GPIO_TogglePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	uint32_t tempODRRegister = GPIOx->ODR;

	GPIOx->BSRR = ( (tempODRRegister & pinNumber) << 16u) | ( ~tempODRRegister & pinNumber);
}




























