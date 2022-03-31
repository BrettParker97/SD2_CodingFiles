################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Bretts_Helper/Src/InnerRing.c \
../Drivers/Bretts_Helper/Src/NoObject.c \
../Drivers/Bretts_Helper/Src/OuterRing.c \
../Drivers/Bretts_Helper/Src/bretts_helper.c 

OBJS += \
./Drivers/Bretts_Helper/Src/InnerRing.o \
./Drivers/Bretts_Helper/Src/NoObject.o \
./Drivers/Bretts_Helper/Src/OuterRing.o \
./Drivers/Bretts_Helper/Src/bretts_helper.o 

C_DEPS += \
./Drivers/Bretts_Helper/Src/InnerRing.d \
./Drivers/Bretts_Helper/Src/NoObject.d \
./Drivers/Bretts_Helper/Src/OuterRing.d \
./Drivers/Bretts_Helper/Src/bretts_helper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Bretts_Helper/Src/%.o: ../Drivers/Bretts_Helper/Src/%.c Drivers/Bretts_Helper/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/MainCode_Examples/Drivers/VL53L0x/Api/core/inc" -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/MainCode_Examples/Drivers/VL53L0x/Api/platform/inc" -I"C:/Users/Brett/Desktop/School/SD2/IRFiles/getUARTworking/SD2_CodingFiles/STM32IDE_Examples/MainCode_Examples/Drivers/Bretts_Helper/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Bretts_Helper-2f-Src

clean-Drivers-2f-Bretts_Helper-2f-Src:
	-$(RM) ./Drivers/Bretts_Helper/Src/InnerRing.d ./Drivers/Bretts_Helper/Src/InnerRing.o ./Drivers/Bretts_Helper/Src/NoObject.d ./Drivers/Bretts_Helper/Src/NoObject.o ./Drivers/Bretts_Helper/Src/OuterRing.d ./Drivers/Bretts_Helper/Src/OuterRing.o ./Drivers/Bretts_Helper/Src/bretts_helper.d ./Drivers/Bretts_Helper/Src/bretts_helper.o

.PHONY: clean-Drivers-2f-Bretts_Helper-2f-Src
