################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bram_mem_test_example.c \
../src/ddrx_mem_test_example.c \
../src/hello_flash.c \
../src/hello_uart.c \
../src/lcd_complete.c \
../src/lcd_simple.c \
../src/menu.c \
../src/platform.c \
../src/push_button_test.c \
../src/rotary_simple.c \
../src/sd_funcs.c \
../src/sd_test.c \
../src/xaxiethernet_example_intr_sgdma.c \
../src/xaxiethernet_example_util_14_5.c \
../src/xaxiethernet_example_util_14_6.c \
../src/xgpio_tapp_example.c \
../src/xiic_eeprom_example.c \
../src/xilflash_protection_example.c \
../src/xsysmon_intr_example.c \
../src/xtmrctr_intr_example.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/bram_mem_test_example.o \
./src/ddrx_mem_test_example.o \
./src/hello_flash.o \
./src/hello_uart.o \
./src/lcd_complete.o \
./src/lcd_simple.o \
./src/menu.o \
./src/platform.o \
./src/push_button_test.o \
./src/rotary_simple.o \
./src/sd_funcs.o \
./src/sd_test.o \
./src/xaxiethernet_example_intr_sgdma.o \
./src/xaxiethernet_example_util_14_5.o \
./src/xaxiethernet_example_util_14_6.o \
./src/xgpio_tapp_example.o \
./src/xiic_eeprom_example.o \
./src/xilflash_protection_example.o \
./src/xsysmon_intr_example.o \
./src/xtmrctr_intr_example.o 

C_DEPS += \
./src/bram_mem_test_example.d \
./src/ddrx_mem_test_example.d \
./src/hello_flash.d \
./src/hello_uart.d \
./src/lcd_complete.d \
./src/lcd_simple.d \
./src/menu.d \
./src/platform.d \
./src/push_button_test.d \
./src/rotary_simple.d \
./src/sd_funcs.d \
./src/sd_test.d \
./src/xaxiethernet_example_intr_sgdma.d \
./src/xaxiethernet_example_util_14_5.d \
./src/xaxiethernet_example_util_14_6.d \
./src/xgpio_tapp_example.d \
./src/xiic_eeprom_example.d \
./src/xilflash_protection_example.d \
./src/xsysmon_intr_example.d \
./src/xtmrctr_intr_example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../board_test_app_Console_platform/microblaze_subsystem_microblaze_0/include -mlittle-endian -mcpu=v9.1 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


