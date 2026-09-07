#include "stm32f407xx.h"

USART_HandleTypeDef_t USART_Handle;

static void GPIO_Config(void);
static void UART_Config(void);

int main(void)
{
    uint8_t txData = 'A';
    volatile uint8_t rxData = 0;

    GPIO_Config();
    UART_Config();

    USART_TransmitData(&USART_Handle, &txData, 1);

    USART_ReceiveData(&USART_Handle, (uint8_t *)&rxData, 1);

    while (1)
    {
    }
}

static void UART_Config()
{

	RCC_USART2_CLK_ENABLE();

	USART_Handle.Instance = USART2;
	USART_Handle.Init.BaudRate = 9600;
	USART_Handle.Init.HardwareFlowControl = USART_HW_NONE;
	USART_Handle.Init.Mode = USART_MODE_Tx_Rx;
	USART_Handle.Init.OverSampling = USART_OVERSAMPLE_16;
	USART_Handle.Init.Parity = USART_PARITY_NONE;
	USART_Handle.Init.StopBits = USART_STOPBITS_1;
	USART_Handle.Init.WordLength = USART_WORDLENGTH_8Bits;

	USART_Init(&USART_Handle);

	USART_PeriphCmd(&USART_Handle, ENABLE);

}

static void GPIO_Config()
{
	GPIO_InitTypeDef_t GPIO_InitStruct = { 0 };

	RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.pinNumber = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF7;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}
