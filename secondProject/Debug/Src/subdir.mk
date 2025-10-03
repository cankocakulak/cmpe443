################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/breadboard_experiment.c \
../Src/main.c \
../Src/simple_experiment.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/breadboard_experiment.o \
./Src/main.o \
./Src/simple_experiment.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/breadboard_experiment.d \
./Src/main.d \
./Src/simple_experiment.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DNUCLEO_L552ZE_Q -DSTM32 -DSTM32L5 -DSTM32L552ZETxQ -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/breadboard_experiment.cyclo ./Src/breadboard_experiment.d ./Src/breadboard_experiment.o ./Src/breadboard_experiment.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/simple_experiment.cyclo ./Src/simple_experiment.d ./Src/simple_experiment.o ./Src/simple_experiment.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

