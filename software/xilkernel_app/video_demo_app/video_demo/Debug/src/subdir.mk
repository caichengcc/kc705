################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SwConfig_xilkernel.c \
../src/dispatch.c \
../src/hdmi_init.c \
../src/http_response.c \
../src/logiCVC_demo.c \
../src/logiCVC_mainXilinx.c \
../src/logiSDHC_fatfs_example.c \
../src/main.c \
../src/platform.c \
../src/platform_fs.c \
../src/vdemo_cvc.c \
../src/vdemo_main.c \
../src/vdemo_perf_monitor.c \
../src/vdemo_perf_monitor_noDrv.c \
../src/vdemo_timebase.c \
../src/vdemo_vdma.c \
../src/web_utils.c \
../src/webserver.c \
../src/xsysmon_intr_example.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/SwConfig_xilkernel.o \
./src/dispatch.o \
./src/hdmi_init.o \
./src/http_response.o \
./src/logiCVC_demo.o \
./src/logiCVC_mainXilinx.o \
./src/logiSDHC_fatfs_example.o \
./src/main.o \
./src/platform.o \
./src/platform_fs.o \
./src/vdemo_cvc.o \
./src/vdemo_main.o \
./src/vdemo_perf_monitor.o \
./src/vdemo_perf_monitor_noDrv.o \
./src/vdemo_timebase.o \
./src/vdemo_vdma.o \
./src/web_utils.o \
./src/webserver.o \
./src/xsysmon_intr_example.o 

C_DEPS += \
./src/SwConfig_xilkernel.d \
./src/dispatch.d \
./src/hdmi_init.d \
./src/http_response.d \
./src/logiCVC_demo.d \
./src/logiCVC_mainXilinx.d \
./src/logiSDHC_fatfs_example.d \
./src/main.d \
./src/platform.d \
./src/platform_fs.d \
./src/vdemo_cvc.d \
./src/vdemo_main.d \
./src/vdemo_perf_monitor.d \
./src/vdemo_perf_monitor_noDrv.d \
./src/vdemo_timebase.d \
./src/vdemo_vdma.d \
./src/web_utils.d \
./src/webserver.d \
./src/xsysmon_intr_example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -D __XMK__ -I../../xilkernel_bsp_0/microblaze_subsystem_microblaze_0/include -mlittle-endian -mcpu=v9.1 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


