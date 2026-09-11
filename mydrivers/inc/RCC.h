#ifndef INC_RCC_H_
#define INC_RCC_H_

#include "stm32f407xx.h"

/*
 *
 *
 * RCC AHB1 Peripheral Clock Control Macro Definitions
 *
 *
 */

#define RCC_GPIOA_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);	\
												tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_GPIOB_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);	\
												tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_GPIOC_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);	\
												tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_GPIOD_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);	\
												tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_SYSCFG_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);	\
												tempValue = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)


#define RCC_GPIOA_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN)
#define RCC_GPIOB_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN)
#define RCC_GPIOC_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN)
#define RCC_GPIOD_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN)

/*
 *
 *
 * RCC APB1 Peripheral Clock Control Macro Definitions
 *
 *
 */

#define RCC_SPI2_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);	\
												tempValue = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_SPI3_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI3EN);	\
												tempValue = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI3EN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_SPI2_CLK_DISABLE()			CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN)
#define RCC_SPI3_CLK_DISABLE()			CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI3EN)


#define RCC_USART2_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2);	\
												tempValue = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_USART2_CLK_DISABLE()			CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2)



#define RCC_I2C1_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);	\
												tempValue = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)

#define RCC_I2C1_CLK_DISABLE()				CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN)

/*
 *
 *
 * RCC APB2 Peripheral Clock Control Macro Definitions
 *
 *
 */

#define RCC_SPI1_CLK_ENABLE()			do{ 	uint32_t tempValue = 0;	\
												SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);	\
												tempValue = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); /* Giving time for setting bits*/ \
												UNUSED(tempValue);	\
											}while(0)


#define RCC_SPI1_CLK_DISABLE()			CLEAR_BIT(RCC->AHB2ENR, RCC_APB2ENR_SPI1EN)

uint32_t RCC_GetSystemClock();
uint32_t RCC_GetHClock();
uint32_t RCC_GetPClock1();
uint32_t RCC_GetPClock2();


#endif /* INC_RCC_H_ */
