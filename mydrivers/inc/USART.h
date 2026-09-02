#ifndef INC_USART_H_
#define INC_USART_H_

#include "stm32f407xx.h"

/*
 *
 * @def_group MODE_Types
 *
 */

#define USART_MODE_Tx				((uint32_t)(0x0000008))
#define USART_MODE_Rx				((uint32_t)(0x0000004))
#define USART_MODE_Tx_Rx			((uint32_t)(0x000000C))

/*
 *
 * @def_group WordLength_Types
 *
 */

#define USART_WORLDLENGTH_8Bits		((uint32_t)(0x0000000))
#define USART_WORLDLENGTH_9Bits		((uint32_t)(0x0001000))

/*
 *
 * @def_group Parity_Modes
 *
 */

#define USART_PARITY_NONE			((uint32_t)(0x0000000))
#define USART_PARITY_EVEN			((uint32_t)(0x0000400))
#define USART_PARITY_ODD			((uint32_t)(0x0000600))

typedef struct
{
	uint32_t Mode;					/*!< Transmission and Reception Modes @def_group MODE_Types */
	uint32_t BaudRate;
	uint32_t WordLength;			/*!< 8 Bits & 9 Bits Modes @def_group WordLength_Types */
	uint32_t Parity;				/*!< Even & Odd Modes @def_group Parity_Modes */
	uint32_t StopBits;
	uint32_t OverSampling;
	uint32_t HardwareFlowControl;

}USART_InitTypeDef_t;

typedef struct
{
	USART_TypeDef_t		*Instance;
	USART_InitTypeDef_t	Init;

}USART_HandleTypeDef_t;







#endif /* INC_USART_H_ */
