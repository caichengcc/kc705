################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
video_demo.elf.size \

ELFCHECK += \
video_demo.elf.elfcheck \


# All Target
all: video_demo.elf secondary-outputs

# Tool invocations
video_demo.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MicroBlaze gcc linker'
	mb-gcc -L../../memfs -Wl,-T -Wl,../src/lscript.ld -L../../xilkernel_bsp_0/microblaze_subsystem_microblaze_0/lib -mlittle-endian -mcpu=v9.1 -mxl-soft-mul -o "video_demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

video_demo.elf.size: video_demo.elf
	@echo 'Invoking: MicroBlaze Print Size'
	mb-size video_demo.elf  |tee "video_demo.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

video_demo.elf.elfcheck: video_demo.elf
	@echo 'Invoking: Xilinx ELF Check'
	elfcheck video_demo.elf -hw ../../hw_platform/system.xml -pe microblaze_subsystem_microblaze_0  |tee "video_demo.elf.elfcheck"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) video_demo.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
