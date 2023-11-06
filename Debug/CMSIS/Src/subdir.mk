################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/Src/system_stm32f0xx.c 

OBJS += \
./CMSIS/Src/system_stm32f0xx.o 

C_DEPS += \
./CMSIS/Src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/Src/%.o CMSIS/Src/%.su CMSIS/Src/%.cyclo: ../CMSIS/Src/%.c CMSIS/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F070CBTx -c -I"C:/Users/proim/OneDrive/STM/STMF0ws2812NoHAL/Inc" -I"C:/Users/proim/OneDrive/STM/STMF0ws2812NoHAL/CMSIS/Inc" -I"C:/Users/proim/OneDrive/STM/STMF0ws2812NoHAL/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CMSIS-2f-Src

clean-CMSIS-2f-Src:
	-$(RM) ./CMSIS/Src/system_stm32f0xx.cyclo ./CMSIS/Src/system_stm32f0xx.d ./CMSIS/Src/system_stm32f0xx.o ./CMSIS/Src/system_stm32f0xx.su

.PHONY: clean-CMSIS-2f-Src

