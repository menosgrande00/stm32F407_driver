#include "stm32f407xx.h"

static void GPIO_LedConfig();
static void	GPIO_ButtonInterruptConfig();
static void	SPI_Config();
static void SPI_GPIO_Config();

SPI_HandleTypeDef_t SPI_Handle;
USART_HandleTypeDef_t USART_Handle;

uint8_t spiTxData[] = "HELLO WORLD\n";
uint8_t spiRxData[sizeof(spiTxData)] = {0};

void EXTI0_IRQHandler()
{
    if (EXTI->PR & 0x1U)
    {
        EXTI->PR |= (0x1U << 0U);

        SPI_ReceiveData_IT(&SPI_Handle, spiRxData, sizeof(spiTxData)); //First, receive need to say i am ready.

        SPI_TransmitData_IT(&SPI_Handle, spiTxData, sizeof(spiTxData));
    }
}

void	SPI1_IRQHandler()
{
	SPI_InterruptHandler(&SPI_Handle);
}

int main(void)
{
    GPIO_LedConfig();

    SPI_GPIO_Config();
    SPI_Config();

    GPIO_ButtonInterruptConfig();


    for (;;)
    {

    }
}

static void GPIO_LedConfig()
{
	GPIO_InitTypeDef_t GPIO_InitStruct =
	{ 0 };

	RCC_GPIOD_CLK_ENABLE();
	RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.pinNumber = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
			| GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));

	GPIO_InitStruct.pinNumber = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.PuPd = GPIO_PUPD_PULLDOWN;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static void	GPIO_ButtonInterruptConfig()
{
	EXTI_InitTypeDef_t EXTI_InitStruct = { 0 };

	RCC_SYSCFG_CLK_ENABLE();

	EXTI_LineConfig(EXTI_PortSource_GPIOA, EXTI_LineSource_0);

	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Line_Number = EXTI_LineSource_0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.TriggerSelection = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_EnableInterrupt(EXTI0_IRQNumber);

}

static void	SPI_Config()
{
	RCC_SPI1_CLK_ENABLE();

	SPI_Handle.Instance = SPI1;
	SPI_Handle.Init.BaudRate = SPI_BAUD_RATE_DIV16;
	SPI_Handle.Init.Bus_Config = SPI_Bus_FullDublex;
	SPI_Handle.Init.CPHA = SPI_CPHA_FIRST;
	SPI_Handle.Init.CPOL = SPI_CPOL_LOW;
	SPI_Handle.Init.DFF_Format = SPI_DFF_8BITS;
	SPI_Handle.Init.FrameFormat = SPI_FF_FORMAT_MSB;
	SPI_Handle.Init.Mode = SPI_MODE_MASTER;
	SPI_Handle.Init.SSM_Cmd = SPI_SSM_ENABLE;

	SPI_Init(&SPI_Handle);

	NVIC_EnableInterrupt(SPI1_IRQNumber);

	SPI_PeriphCmd(&SPI_Handle, ENABLE);
}

static void SPI_GPIO_Config()
{
	GPIO_InitTypeDef_t GPIO_InitStruct = { 0 };

	RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.pinNumber = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; // PA5 = SCK, PA7 = MOSI
	GPIO_InitStruct.Mode = GPIO_MODE_AF;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF5;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}










