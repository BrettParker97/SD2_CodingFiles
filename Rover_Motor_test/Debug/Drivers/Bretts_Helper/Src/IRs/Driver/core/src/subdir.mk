################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api.c \
../Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_calibration.c \
../Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_core.c \
../Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_ranging.c \
../Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_strings.c 

OBJS += \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api.o \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_calibration.o \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_core.o \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_ranging.o \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_strings.o 

C_DEPS += \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api.d \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_calibration.d \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_core.d \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_ranging.d \
./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_strings.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Bretts_Helper/Src/IRs/Driver/core/src/%.o: ../Drivers/Bretts_Helper/Src/IRs/Driver/core/src/%.c Drivers/Bretts_Helper/Src/IRs/Driver/core/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S9xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IMU" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IRs/Driver/core/inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IRs/Driver/platform/inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/ObjectDetection" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Bretts_Helper-2f-Src-2f-IRs-2f-Driver-2f-core-2f-src

clean-Drivers-2f-Bretts_Helper-2f-Src-2f-IRs-2f-Driver-2f-core-2f-src:
	-$(RM) ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api.d ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api.o ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_calibration.d ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_calibration.o ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_core.d ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_core.o ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_ranging.d ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_ranging.o ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_strings.d ./Drivers/Bretts_Helper/Src/IRs/Driver/core/src/vl53l0x_api_strings.o

.PHONY: clean-Drivers-2f-Bretts_Helper-2f-Src-2f-IRs-2f-Driver-2f-core-2f-src

