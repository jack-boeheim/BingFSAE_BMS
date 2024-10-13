################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ADBMS6830/lib/src/adBms6830GenericType.c \
../Core/ADBMS6830/lib/src/adBms6830ParseCreate.c 

OBJS += \
./Core/ADBMS6830/lib/src/adBms6830GenericType.o \
./Core/ADBMS6830/lib/src/adBms6830ParseCreate.o 

C_DEPS += \
./Core/ADBMS6830/lib/src/adBms6830GenericType.d \
./Core/ADBMS6830/lib/src/adBms6830ParseCreate.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ADBMS6830/lib/src/%.o Core/ADBMS6830/lib/src/%.su Core/ADBMS6830/lib/src/%.cyclo: ../Core/ADBMS6830/lib/src/%.c Core/ADBMS6830/lib/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kevin/STM32CubeIDE/workspace_1.16.1/BingFSAE_BMS/Core/ADBMS6830/lib/inc" -I"C:/Users/kevin/STM32CubeIDE/workspace_1.16.1/BingFSAE_BMS/Core/ADBMS6830/program/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-ADBMS6830-2f-lib-2f-src

clean-Core-2f-ADBMS6830-2f-lib-2f-src:
	-$(RM) ./Core/ADBMS6830/lib/src/adBms6830GenericType.cyclo ./Core/ADBMS6830/lib/src/adBms6830GenericType.d ./Core/ADBMS6830/lib/src/adBms6830GenericType.o ./Core/ADBMS6830/lib/src/adBms6830GenericType.su ./Core/ADBMS6830/lib/src/adBms6830ParseCreate.cyclo ./Core/ADBMS6830/lib/src/adBms6830ParseCreate.d ./Core/ADBMS6830/lib/src/adBms6830ParseCreate.o ./Core/ADBMS6830/lib/src/adBms6830ParseCreate.su

.PHONY: clean-Core-2f-ADBMS6830-2f-lib-2f-src

