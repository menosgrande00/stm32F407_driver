#include "stm32f407xx.h"
#include "Modbus.h"
#include "Modbus_CRC.h"

USART_HandleTypeDef_t USART_Handle;

static void GPIO_Config(void);
static void UART_Config(void);

void USART2_IRQHandler(void)
{
	USART_InterruptHandler(&USART_Handle);
}

int main(void)
{
	uint8_t modbusRxBuffer[MODBUS_READ_REQUEST_SIZE];
	Modbus_FrameStatus_t status;
	ModbusFrame_t frame;

	GPIO_Config();
	UART_Config();
	Modbus_CRC_GenerateTable();

	USART_ReceiveData_IT(&USART_Handle, modbusRxBuffer, MODBUS_READ_REQUEST_SIZE);

	while (1)
	{
		if (USART_Handle.RxStatus == USART_BUS_FREE)
		{
			status = Modbus_ValidateFrame(modbusRxBuffer, MODBUS_READ_REQUEST_SIZE);

			if (status == MODBUS_FRAME_OK)
			{
				Modbus_ParseFrame(modbusRxBuffer, MODBUS_READ_REQUEST_SIZE, &frame);
			}

			USART_ReceiveData_IT(&USART_Handle, modbusRxBuffer, MODBUS_READ_REQUEST_SIZE);
		}
	}
}

static void UART_Config(void)
{
	RCC_USART2_CLK_ENABLE();

	USART_Handle.Instance = USART2;
	USART_Handle.Init.BaudRate = 115200;
	USART_Handle.Init.HardwareFlowControl = USART_HW_NONE;
	USART_Handle.Init.Mode = USART_MODE_Tx_Rx;
	USART_Handle.Init.OverSampling = USART_OVERSAMPLE_16;
	USART_Handle.Init.Parity = USART_PARITY_EVEN;
	USART_Handle.Init.StopBits = USART_STOPBITS_1;
	USART_Handle.Init.WordLength = USART_WORDLENGTH_9Bits;

	USART_Init(&USART_Handle);
	NVIC_EnableInterrupt(USART2_IRQNumber);
	USART_PeriphCmd(&USART_Handle, ENABLE);
}

static void GPIO_Config(void)
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
