################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L0x/Api/platform/src/vl53l0x_platform.c \
../Drivers/VL53L0x/Api/platform/src/vl53l0x_platform_log.c 

OBJS += \
./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform.o \
./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform_log.o 

C_DEPS += \
./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform.d \
./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform_log.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L0x/Api/platform/src/%.o: ../Drivers/VL53L0x/Api/platform/src/%.c Drivers/VL53L0x/Api/platform/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/IR_Examples/MutliSensing/getUARTworking/Drivers/VL53L0x/Api/core/inc" -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/IR_Examples/MutliSensing/getUARTworking/Drivers/VL53L0x/Api/platform/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-VL53L0x-2f-Api-2f-platform-2f-src

clean-Drivers-2f-VL53L0x-2f-Api-2f-platform-2f-src:
	-$(RM) ./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform.d ./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform.o ./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform_log.d ./Drivers/VL53L0x/Api/platform/src/vl53l0x_platform_log.o

.PHONY: clean-Drivers-2f-VL53L0x-2f-Api-2f-platform-2f-src

