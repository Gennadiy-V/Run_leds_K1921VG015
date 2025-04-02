################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/source/plic.c \
../platform/source/printf.c \
../platform/source/sys_init.c \
../platform/source/system_k1921vg015.c 

S_UPPER_SRCS += \
../platform/source/startup_k1921vg015.S 

C_DEPS += \
./platform/source/plic.d \
./platform/source/printf.d \
./platform/source/sys_init.d \
./platform/source/system_k1921vg015.d 

OBJS += \
./platform/source/plic.o \
./platform/source/printf.o \
./platform/source/startup_k1921vg015.o \
./platform/source/sys_init.o \
./platform/source/system_k1921vg015.o 

S_UPPER_DEPS += \
./platform/source/startup_k1921vg015.d 


# Each subdirectory must supply rules for building sources it contributes
platform/source/%.o: ../platform/source/%.c platform/source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc  -march=rv32imfc -mabi=ilp32f  -msmall-data-limit=8 -mstrict-align -mno-save-restore  -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -fno-builtin -flto -DSELF_TIMED=1 -Wall -Wextra -g3 -ggdb -DHSECLK_VAL=16000000 -DRETARGET -DSYSCLK_PLL -DCKO_PLL0 -I../platform/include -I../platform/ldscripts -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

platform/source/%.o: ../platform/source/%.S platform/source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv64-unknown-elf-gcc  -march=rv32imfc -mabi=ilp32f  -msmall-data-limit=8 -mstrict-align -mno-save-restore  -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -fno-builtin -flto -DSELF_TIMED=1 -Wall -Wextra -g3 -ggdb -x assembler-with-cpp -I../platform/include -I../platform/ldscripts -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-platform-2f-source

clean-platform-2f-source:
	-$(RM) ./platform/source/plic.d ./platform/source/plic.o ./platform/source/printf.d ./platform/source/printf.o ./platform/source/startup_k1921vg015.d ./platform/source/startup_k1921vg015.o ./platform/source/sys_init.d ./platform/source/sys_init.o ./platform/source/system_k1921vg015.d ./platform/source/system_k1921vg015.o

.PHONY: clean-platform-2f-source

