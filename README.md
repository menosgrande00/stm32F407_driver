# STM32F407 Custom Peripheral Drivers

A lightweight, register-level driver project for the **STM32F407** microcontroller, written in C for learning and experimenting with low-level peripheral programming without relying on STM32 HAL/LL drivers.

The repository contains custom device/register definitions and peripheral drivers for:

- GPIO
- RCC / clock queries
- EXTI
- SPI
- I2C
- USART

It also includes interrupt-driven SPI and USART usage examples.

> **Project status:** Educational / work in progress. The drivers are intentionally implemented close to the hardware and are not intended to be a drop-in replacement for STM32 HAL or a production-ready driver framework.

---

## Features

### GPIO

Custom GPIO configuration and basic pin operations.

Supported configuration options include:

- Input
- Output
- Alternate Function
- Analog mode
- Push-pull / Open-drain
- Pull-up / Pull-down / No-pull
- Multiple output speed options
- Alternate functions AF0-AF15

Available API includes:

```c
GPIO_Init(...)
GPIO_WritePin(...)
GPIO_ReadPin(...)
GPIO_LockPin(...)
GPIO_TogglePin(...)
```

### RCC

Peripheral clock enable/disable macros are provided for the peripherals currently used by the project.

Clock helper functions:

```c
RCC_GetSystemClock()
RCC_GetHClock()
RCC_GetPClock1()
RCC_GetPClock2()
```

These functions are used by communication drivers when peripheral clock information is required.

### EXTI

External interrupt configuration support for GPIO pins.

Available API:

```c
EXTI_Init(...)
EXTI_LineConfig(...)
NVIC_EnableInterrupt(...)
```

The current example configures **PA0 -> EXTI0** with a rising-edge interrupt.

### SPI

Custom SPI driver with both polling and interrupt-based data transfer.

Supported configuration includes:

- Master / Slave mode
- CPOL
- CPHA
- Baud-rate prescaler
- 8-bit / 16-bit data frame
- MSB / LSB first
- Full-duplex
- Receive-only
- Half-duplex
- Software slave management

Available API:

```c
SPI_Init(...)
SPI_PeriphCmd(...)
SPI_TransmitData(...)
SPI_ReceiveData(...)
SPI_TransmitData_IT(...)
SPI_ReceiveData_IT(...)
SPI_InterruptHandler(...)
SPI_GetFlagStatus(...)
```

### USART

USART driver with polling and interrupt-driven transmit/receive support.

Supported configuration includes:

- TX
- RX
- TX + RX
- 8-bit / 9-bit word length
- Even / Odd / No parity
- Multiple stop-bit configurations
- 8x / 16x oversampling
- CTS / RTS hardware flow control

Available API:

```c
USART_Init(...)
USART_TransmitData(...)
USART_ReceiveData(...)
USART_TransmitData_IT(...)
USART_ReceiveData_IT(...)
USART_PeriphCmd(...)
USART_GetFlagStatus(...)
USART_InterruptHandler(...)
```

### I2C

The current I2C implementation provides peripheral initialization and enable/disable support.

Configuration includes:

- Standard mode: 100 kHz
- Fast mode: 400 kHz
- ACK enable/disable
- Clock stretching
- 7-bit / 10-bit addressing
- Duty-cycle configuration
- Own-address configuration

Current API:

```c
I2C_Init(...)
I2C_PeriphCmd(...)
```

---

## Repository Structure

```text
stm32F407_driver/
├── Debug/
├── Src/
│   ├── main.c
│   ├── USART_Test.c
│   ├── syscalls.c
│   └── sysmem.c
├── Startup/
├── mydrivers/
│   ├── inc/
│   │   ├── EXTI.h
│   │   ├── GPIO.h
│   │   ├── I2C.h
│   │   ├── RCC.h
│   │   ├── SPI.h
│   │   ├── USART.h
│   │   └── stm32f407xx.h
│   └── src/
│       ├── EXTI.c
│       ├── GPIO.c
│       ├── I2C.c
│       ├── RCC.c
│       ├── SPI.c
│       └── USART.c
├── STM32F407VGTX_FLASH.ld
├── STM32F407VGTX_RAM.ld
└── driver_development Debug.launch
```

### Important files

| File | Description |
|---|---|
| `mydrivers/inc/stm32f407xx.h` | STM32F407 memory map, register structures, peripheral addresses and bit definitions |
| `mydrivers/inc/*.h` | Public driver configuration structures, constants and APIs |
| `mydrivers/src/*.c` | Driver implementations |
| `Src/main.c` | SPI + GPIO + EXTI interrupt example |
| `Src/USART_Test.c` | USART2 interrupt TX/RX example |
| `Startup/` | MCU startup code |
| `STM32F407VGTX_FLASH.ld` | Flash linker script |
| `STM32F407VGTX_RAM.ld` | RAM linker script |

---

## Current Main Example

The current `Src/main.c` demonstrates an interrupt-driven SPI setup.

### GPIO configuration

The example configures:

- `GPIOD` pins **12, 13, 14 and 15** as outputs
- `GPIOA pin 0` as an input
- `GPIOA pins 5, 6 and 7` as SPI alternate-function pins

SPI pin configuration:

| Pin | Function |
|---|---|
| PA5 | SPI1 SCK |
| PA6 | SPI1 MISO |
| PA7 | SPI1 MOSI |

The pins use **AF5**.

### SPI1 configuration

The example configures SPI1 as:

```text
Mode        : Master
Bus         : Full duplex
Baud divider: /16
CPOL        : Low
CPHA        : First edge
Frame size  : 8 bit
Bit order   : MSB first
SSM         : Enabled
```

SPI1 interrupt handling is connected through:

```c
void SPI1_IRQHandler(void)
{
    SPI_InterruptHandler(&SPI_Handle);
}
```

### Button interrupt

PA0 is connected to EXTI line 0 and configured for a rising-edge interrupt.

When the interrupt occurs, the example starts interrupt-driven SPI receive and transmit operations:

```c
void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & 0x1U)
    {
        EXTI->PR |= (0x1U << 0U);

        SPI_ReceiveData_IT(&SPI_Handle, spiRxData, sizeof(spiTxData));
        SPI_TransmitData_IT(&SPI_Handle, spiTxData, sizeof(spiTxData));
    }
}
```

---

## USART Example

`Src/USART_Test.c` contains a USART2 interrupt example.

### USART2 pins

| Pin | Function | Alternate Function |
|---|---|---|
| PA2 | USART2 TX | AF7 |
| PA3 | USART2 RX | AF7 |

### USART configuration

```text
Peripheral    : USART2
Baud rate     : 9600
Mode          : TX + RX
Word length   : 8 bit
Parity        : None
Stop bits     : 1
Oversampling  : 16
Flow control  : None
```

Example initialization:

```c
static void UART_Config(void)
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

    NVIC_EnableInterrupt(USART2_IRQNumber);
    USART_PeriphCmd(&USART_Handle, ENABLE);
}
```

Interrupt handler:

```c
void USART2_IRQHandler(void)
{
    USART_InterruptHandler(&USART_Handle);
}
```

The test starts non-blocking transmit and receive operations with:

```c
char msgToSend[] = "Hello World\n";
char receiveData[30];

USART_TransmitData_IT(
    &USART_Handle,
    (uint8_t *)msgToSend,
    strlen(msgToSend)
);

USART_ReceiveData_IT(
    &USART_Handle,
    (uint8_t *)receiveData,
    20
);
```

---

## Basic GPIO Example

```c
#include "stm32f407xx.h"

int main(void)
{
    GPIO_InitTypeDef_t gpio = {0};

    RCC_GPIOD_CLK_ENABLE();

    gpio.pinNumber = GPIO_PIN_12;
    gpio.Mode       = GPIO_MODE_OUTPUT;
    gpio.Otype      = GPIO_OTYPE_PP;
    gpio.PuPd       = GPIO_PUPD_NOPULL;
    gpio.Speed      = GPIO_SPEED_LOW;

    GPIO_Init(GPIOD, &gpio);

    while (1)
    {
        GPIO_TogglePin(GPIOD, GPIO_PIN_12);

        for (volatile uint32_t i = 0; i < 500000; ++i)
        {
        }
    }
}
```

---

## How to Use

### 1. Clone the repository

```bash
git clone https://github.com/menosgrande00/stm32F407_driver.git
cd stm32F407_driver
```

### 2. Add the driver include directory

Make sure the compiler include path contains:

```text
mydrivers/inc
```

### 3. Compile the driver sources

Add the required files from:

```text
mydrivers/src
```

For example, an SPI application normally requires at least:

```text
GPIO.c
RCC.c
SPI.c
EXTI.c
```

depending on which parts of the project are used.

### 4. Enable peripheral clocks

Before configuring a peripheral, enable its clock.

Example:

```c
RCC_GPIOA_CLK_ENABLE();
RCC_SPI1_CLK_ENABLE();
```

### 5. Configure GPIO alternate functions

Communication peripherals require the correct GPIO alternate-function mapping.

Example for SPI1:

```c
GPIO_InitTypeDef_t gpio = {0};

RCC_GPIOA_CLK_ENABLE();

gpio.pinNumber = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
gpio.Mode      = GPIO_MODE_AF;
gpio.Otype     = GPIO_OTYPE_PP;
gpio.PuPd      = GPIO_PUPD_NOPULL;
gpio.Speed     = GPIO_SPEED_MEDIUM;
gpio.Alternate = GPIO_AF5;

GPIO_Init(GPIOA, &gpio);
```

### 6. Configure and enable the peripheral

Example:

```c
SPI_HandleTypeDef_t spi = {0};

RCC_SPI1_CLK_ENABLE();

spi.Instance         = SPI1;
spi.Init.BaudRate    = SPI_BAUD_RATE_DIV16;
spi.Init.Bus_Config  = SPI_Bus_FullDublex;
spi.Init.CPHA        = SPI_CPHA_FIRST;
spi.Init.CPOL        = SPI_CPOL_LOW;
spi.Init.DFF_Format  = SPI_DFF_8BITS;
spi.Init.FrameFormat = SPI_FF_FORMAT_MSB;
spi.Init.Mode        = SPI_MODE_MASTER;
spi.Init.SSM_Cmd     = SPI_SSM_ENABLE;

SPI_Init(&spi);
SPI_PeriphCmd(&spi, ENABLE);
```

---

## Build Environment

The repository contains STM32F407VGTX linker scripts, startup code and a debug launch configuration.

A typical toolchain can include:

- ARM GCC (`arm-none-eabi-gcc`)
- STM32CubeIDE / Eclipse-based embedded environment
- ST-LINK for flashing and debugging

There is currently no portable Makefile or CMake configuration in the repository, so project/build configuration may need to be created or imported manually depending on the development environment.

---

## Design Approach

This project intentionally works close to the MCU registers.

Instead of using STM32 HAL calls such as:

```c
HAL_GPIO_WritePin(...);
HAL_SPI_Transmit(...);
```

the project defines its own:

- Peripheral register structures
- Base addresses
- Bit masks
- Initialization structures
- Driver APIs
- Interrupt handlers

This makes the repository useful for understanding how STM32 peripheral drivers work underneath higher-level libraries.

---

## Notes

- Target MCU/device configuration is based on **STM32F407VGTX**.
- The code is primarily intended for educational purposes and low-level driver development.
- The I2C driver currently focuses on peripheral configuration; transfer APIs are not yet exposed.
- The custom device header only defines the interrupt numbers and register/bit definitions currently needed by the project, so additional peripherals may require extending it.
- Verify pin mappings, clocks and electrical connections for your specific STM32F407 board before running an example.
- For production applications, additional error handling, timeout handling and broader device validation would be recommended.

---

## Roadmap Ideas

Possible future improvements:

- Complete I2C master transmit/receive APIs
- Add I2C interrupt support
- Add USART callback/event handling
- Add SPI completion callbacks
- Extend NVIC configuration
- Add timer drivers
- Add ADC / DAC drivers
- Add DMA support
- Add unit/integration examples for each peripheral
- Add CMake or Makefile build support
- Add automated tests and CI

---

## Contributing

Issues, improvements and pull requests are welcome.

When adding a new peripheral driver, keeping the existing structure is recommended:

```text
mydrivers/inc/<PERIPHERAL>.h
mydrivers/src/<PERIPHERAL>.c
```

Try to keep register manipulation inside the driver implementation and expose a small configuration/API layer through the corresponding header.

---

## License

No license file is currently included in this repository.

If this project is intended for public reuse or contribution, consider adding an explicit open-source license such as MIT, BSD-3-Clause, or another license appropriate for the project.
