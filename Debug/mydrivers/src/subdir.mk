################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mydrivers/src/EXTI.c \
../mydrivers/src/GPIO.c \
../mydrivers/src/I2C.c \
../mydrivers/src/ModBus_CRC.c \
../mydrivers/src/Modbus.c \
../mydrivers/src/RCC.c \
../mydrivers/src/SPI.c \
../mydrivers/src/USART.c 

OBJS += \
./mydrivers/src/EXTI.o \
./mydrivers/src/GPIO.o \
./mydrivers/src/I2C.o \
./mydrivers/src/ModBus_CRC.o \
./mydrivers/src/Modbus.o \
./mydrivers/src/RCC.o \
./mydrivers/src/SPI.o \
./mydrivers/src/USART.o 

C_DEPS += \
./mydrivers/src/EXTI.d \
./mydrivers/src/GPIO.d \
./mydrivers/src/I2C.d \
./mydrivers/src/ModBus_CRC.d \
./mydrivers/src/Modbus.d \
./mydrivers/src/RCC.d \
./mydrivers/src/SPI.d \
./mydrivers/src/USART.d 


# Each subdirectory must supply rules for building sources it contributes
mydrivers/src/%.o mydrivers/src/%.su mydrivers/src/%.cyclo: ../mydrivers/src/%.c mydrivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"/Users/omerfarukonal/STM32CubeIDE/workspace_2.2.0/driver_development/mydrivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-mydrivers-2f-src

clean-mydrivers-2f-src:
	-$(RM) ./mydrivers/src/EXTI.cyclo ./mydrivers/src/EXTI.d ./mydrivers/src/EXTI.o ./mydrivers/src/EXTI.su ./mydrivers/src/GPIO.cyclo ./mydrivers/src/GPIO.d ./mydrivers/src/GPIO.o ./mydrivers/src/GPIO.su ./mydrivers/src/I2C.cyclo ./mydrivers/src/I2C.d ./mydrivers/src/I2C.o ./mydrivers/src/I2C.su ./mydrivers/src/ModBus_CRC.cyclo ./mydrivers/src/ModBus_CRC.d ./mydrivers/src/ModBus_CRC.o ./mydrivers/src/ModBus_CRC.su ./mydrivers/src/Modbus.cyclo ./mydrivers/src/Modbus.d ./mydrivers/src/Modbus.o ./mydrivers/src/Modbus.su ./mydrivers/src/RCC.cyclo ./mydrivers/src/RCC.d ./mydrivers/src/RCC.o ./mydrivers/src/RCC.su ./mydrivers/src/SPI.cyclo ./mydrivers/src/SPI.d ./mydrivers/src/SPI.o ./mydrivers/src/SPI.su ./mydrivers/src/USART.cyclo ./mydrivers/src/USART.d ./mydrivers/src/USART.o ./mydrivers/src/USART.su

.PHONY: clean-mydrivers-2f-src

