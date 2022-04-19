################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Bretts_Helper/Src/bretts_helper.c \
../Drivers/Bretts_Helper/Src/clock_rtc.c \
../Drivers/Bretts_Helper/Src/helpful_i2c.c \
../Drivers/Bretts_Helper/Src/motor_test.c 

OBJS += \
./Drivers/Bretts_Helper/Src/bretts_helper.o \
./Drivers/Bretts_Helper/Src/clock_rtc.o \
./Drivers/Bretts_Helper/Src/helpful_i2c.o \
./Drivers/Bretts_Helper/Src/motor_test.o 

C_DEPS += \
./Drivers/Bretts_Helper/Src/bretts_helper.d \
./Drivers/Bretts_Helper/Src/clock_rtc.d \
./Drivers/Bretts_Helper/Src/helpful_i2c.d \
./Drivers/Bretts_Helper/Src/motor_test.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Bretts_Helper/Src/%.o: ../Drivers/Bretts_Helper/Src/%.c Drivers/Bretts_Helper/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S9xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IMU" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IRs/Driver/core/inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IRs/Driver/platform/inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/ObjectDetection" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Bretts_Helper-2f-Src

clean-Drivers-2f-Bretts_Helper-2f-Src:
	-$(RM) ./Drivers/Bretts_Helper/Src/bretts_helper.d ./Drivers/Bretts_Helper/Src/bretts_helper.o ./Drivers/Bretts_Helper/Src/clock_rtc.d ./Drivers/Bretts_Helper/Src/clock_rtc.o ./Drivers/Bretts_Helper/Src/helpful_i2c.d ./Drivers/Bretts_Helper/Src/helpful_i2c.o ./Drivers/Bretts_Helper/Src/motor_test.d ./Drivers/Bretts_Helper/Src/motor_test.o

.PHONY: clean-Drivers-2f-Bretts_Helper-2f-Src

