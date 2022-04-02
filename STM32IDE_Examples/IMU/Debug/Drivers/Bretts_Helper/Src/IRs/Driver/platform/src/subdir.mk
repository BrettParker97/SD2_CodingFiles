################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform.c \
../Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform_log.c 

OBJS += \
./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform.o \
./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform_log.o 

C_DEPS += \
./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform.d \
./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform_log.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/%.o: ../Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/%.c Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/IMU/Drivers/Bretts_Helper/Inc" -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/IMU/Drivers/Bretts_Helper/Inc/IRs/Driver/core/inc" -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/IMU/Drivers/Bretts_Helper/Inc/IRs/Driver/platform/inc" -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/IMU/Drivers/Bretts_Helper/Inc/ObjectDetection" -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/IMU/Drivers/Bretts_Helper/Inc/IMU" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Bretts_Helper-2f-Src-2f-IRs-2f-Driver-2f-platform-2f-src

clean-Drivers-2f-Bretts_Helper-2f-Src-2f-IRs-2f-Driver-2f-platform-2f-src:
	-$(RM) ./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform.d ./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform.o ./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform_log.d ./Drivers/Bretts_Helper/Src/IRs/Driver/platform/src/vl53l0x_platform_log.o

.PHONY: clean-Drivers-2f-Bretts_Helper-2f-Src-2f-IRs-2f-Driver-2f-platform-2f-src

