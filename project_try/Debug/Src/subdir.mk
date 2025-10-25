################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/practice1_gpio.c \
../Src/practice2_timers.c \
../Src/practice3_adc.c \
../Src/project.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/main.o \
./Src/practice1_gpio.o \
./Src/practice2_timers.o \
./Src/practice3_adc.o \
./Src/project.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main.d \
./Src/practice1_gpio.d \
./Src/practice2_timers.d \
./Src/practice3_adc.d \
./Src/project.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DNUCLEO_L552ZE_Q -DSTM32 -DSTM32L5 -DSTM32L552ZETxQ -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/practice1_gpio.cyclo ./Src/practice1_gpio.d ./Src/practice1_gpio.o ./Src/practice1_gpio.su ./Src/practice2_timers.cyclo ./Src/practice2_timers.d ./Src/practice2_timers.o ./Src/practice2_timers.su ./Src/practice3_adc.cyclo ./Src/practice3_adc.d ./Src/practice3_adc.o ./Src/practice3_adc.su ./Src/project.cyclo ./Src/project.d ./Src/project.o ./Src/project.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

