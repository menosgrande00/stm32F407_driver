#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f407xx.h"

#define	__Check_PClockValue(__pClock__, __I2C_Clock__)		((__I2C_Clock__ <= I2C_SPEED_Standart) ? (__pClock__ <= 2000000U) : (__pClock__ <= 4000000U))
#define I2C_Get_FreqValue(__pClock__)						(__pClock__ / (1000000U))

/*
 *
 * @def_gruop I2C_Clock_Speeds
 *
 */

#define I2C_SPEED_Standart			(100000U)
#define I2C_SPEED_Fast				(400000U)

/*
 *
 * @def_gruop I2C_ACK_State
 *
 */

#define I2C_ACK_DISABLE				(0x00000000U)
#define I2C_ACK_ENABLE				(0x00000400U)

/*
 *
 * @def_gruop I2C_Stretch_State
 *
 */

#define I2C_STRETCH_ENABLE			(0x00000000U)
#define I2C_STRETCH_DISABLE			(0x00000080U)

/*
 *
 * @def_gruop I2C_Address_Mode
 *
 */

#define I2C_ADDR_MODE_7				(0x00004000U)
#define I2C_ADDR_MODE_10			(0x0000C000U)

/*
 *
 * @def_gruop I2C_Duty_Mod
 *
 */

#define I2C_DUTY_Standart			(0x00000000U)
#define I2C_DUTY_FM_2				(0x00008000U)
#define I2C_DUTY_FM_16_9			(0x0000C000U)

typedef struct
{
	uint32_t	ClockSpeed;			/*!< I2C Clock Speed Choice @def_gruop I2C_Clock_Speeds			*/
	uint32_t	ACK_State;			/*!< I2C Ack State Choice @def_gruop I2C_ACK_State				*/
	uint32_t	ClockStretch;		/*!< I2C Stretch State Choice @def_gruop I2C_Stretch_State		*/
	uint32_t	AddressingMode;		/*!< I2C Address Mode Choice @def_gruop I2C_Address_Mode		*/
	uint32_t	DutyCycle;			/*!< I2C Duty Mode Choice @def_gruop I2C_Duty_Mode				*/
	uint32_t	MyOwnAddress;		/*!< I2C Slace Mode Address										*/
}I2C_InitTypeDef_t;

typedef struct
{
	I2C_InitTypeDef_t	Init;
	I2C_TypeDef_t		*Instance;
}I2C_HandleTypeDef_t;

void I2C_Init(I2C_HandleTypeDef_t *I2C_Handle);
void I2C_PeriphCmd(I2C_TypeDef_t *I2Cx, FunctionalState_t stateOfI2C);



#endif /* INC_I2C_H_ */
