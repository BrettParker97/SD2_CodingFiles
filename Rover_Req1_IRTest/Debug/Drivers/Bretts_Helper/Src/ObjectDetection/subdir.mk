################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Bretts_Helper/Src/ObjectDetection/InnerRing.c \
../Drivers/Bretts_Helper/Src/ObjectDetection/NoObject.c \
../Drivers/Bretts_Helper/Src/ObjectDetection/OuterRing.c 

OBJS += \
./Drivers/Bretts_Helper/Src/ObjectDetection/InnerRing.o \
./Drivers/Bretts_Helper/Src/ObjectDetection/NoObject.o \
./Drivers/Bretts_Helper/Src/ObjectDetection/OuterRing.o 

C_DEPS += \
./Drivers/Bretts_Helper/Src/ObjectDetection/InnerRing.d \
./Drivers/Bretts_Helper/Src/ObjectDetection/NoObject.d \
./Drivers/Bretts_Helper/Src/ObjectDetection/OuterRing.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Bretts_Helper/Src/ObjectDetection/%.o: ../Drivers/Bretts_Helper/Src/ObjectDetection/%.c Drivers/Bretts_Helper/Src/ObjectDetection/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S9xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE_workspace/Rover_Req1_IRTest/Drivers/Bretts_Helper/Inc" -I"D:/STM32CubeIDE_workspace/Rover_Req1_IRTest/Drivers/Bretts_Helper/Inc/IMU" -I"D:/STM32CubeIDE_workspace/Rover_Req1_IRTest/Drivers/Bretts_Helper/Inc/IRs/Driver/core/inc" -I"D:/STM32CubeIDE_workspace/Rover_Req1_IRTest/Drivers/Bretts_Helper/Inc/IRs/Driver/platform/inc" -I"D:/STM32CubeIDE_workspace/Rover_Req1_IRTest/Drivers/Bretts_Helper/Inc/ObjectDetection" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Bretts_Helper-2f-Src-2f-ObjectDetection

clean-Drivers-2f-Bretts_Helper-2f-Src-2f-ObjectDetection:
	-$(RM) ./Drivers/Bretts_Helper/Src/ObjectDetection/InnerRing.d ./Drivers/Bretts_Helper/Src/ObjectDetection/InnerRing.o ./Drivers/Bretts_Helper/Src/ObjectDetection/NoObject.d ./Drivers/Bretts_Helper/Src/ObjectDetection/NoObject.o ./Drivers/Bretts_Helper/Src/ObjectDetection/OuterRing.d ./Drivers/Bretts_Helper/Src/ObjectDetection/OuterRing.o

.PHONY: clean-Drivers-2f-Bretts_Helper-2f-Src-2f-ObjectDetection

