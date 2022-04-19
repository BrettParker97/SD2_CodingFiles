################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32l4s9zitx.s 

OBJS += \
./Core/Startup/startup_stm32l4s9zitx.o 

S_DEPS += \
./Core/Startup/startup_stm32l4s9zitx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IMU" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IRs/Driver/core/inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/IRs/Driver/platform/inc" -I"D:/STM32CubeIDE_workspace/Rover_Motor_test/Drivers/Bretts_Helper/Inc/ObjectDetection" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32l4s9zitx.d ./Core/Startup/startup_stm32l4s9zitx.o

.PHONY: clean-Core-2f-Startup

