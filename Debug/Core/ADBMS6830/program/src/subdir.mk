################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ADBMS6830/program/src/adBms_Application.c \
../Core/ADBMS6830/program/src/mcuWrapper.c \
../Core/ADBMS6830/program/src/serialPrintResult.c 

OBJS += \
./Core/ADBMS6830/program/src/adBms_Application.o \
./Core/ADBMS6830/program/src/mcuWrapper.o \
./Core/ADBMS6830/program/src/serialPrintResult.o 

C_DEPS += \
./Core/ADBMS6830/program/src/adBms_Application.d \
./Core/ADBMS6830/program/src/mcuWrapper.d \
./Core/ADBMS6830/program/src/serialPrintResult.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ADBMS6830/program/src/%.o Core/ADBMS6830/program/src/%.su Core/ADBMS6830/program/src/%.cyclo: ../Core/ADBMS6830/program/src/%.c Core/ADBMS6830/program/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/kevin/STM32CubeIDE/workspace_1.16.1/BingFSAE_BMS/Core/ADBMS6830/lib/inc" -I"C:/Users/kevin/STM32CubeIDE/workspace_1.16.1/BingFSAE_BMS/Core/ADBMS6830/program/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-ADBMS6830-2f-program-2f-src

clean-Core-2f-ADBMS6830-2f-program-2f-src:
	-$(RM) ./Core/ADBMS6830/program/src/adBms_Application.cyclo ./Core/ADBMS6830/program/src/adBms_Application.d ./Core/ADBMS6830/program/src/adBms_Application.o ./Core/ADBMS6830/program/src/adBms_Application.su ./Core/ADBMS6830/program/src/mcuWrapper.cyclo ./Core/ADBMS6830/program/src/mcuWrapper.d ./Core/ADBMS6830/program/src/mcuWrapper.o ./Core/ADBMS6830/program/src/mcuWrapper.su ./Core/ADBMS6830/program/src/serialPrintResult.cyclo ./Core/ADBMS6830/program/src/serialPrintResult.d ./Core/ADBMS6830/program/src/serialPrintResult.o ./Core/ADBMS6830/program/src/serialPrintResult.su

.PHONY: clean-Core-2f-ADBMS6830-2f-program-2f-src

