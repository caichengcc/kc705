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
board_test_app_Console.elf.size \

ELFCHECK += \
board_test_app_Console.elf.elfcheck \


# All Target
all: board_test_app_Console.elf secondary-outputs

# Tool invocations
board_test_app_Console.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MicroBlaze gcc linker'
	mb-gcc -Wl,-T -Wl,../src/lscript.ld -L../../board_test_app_Console_platform/microblaze_subsystem_microblaze_0/lib -mlittle-endian -mcpu=v9.1 -mxl-soft-mul -o "board_test_app_Console.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

board_test_app_Console.elf.size: board_test_app_Console.elf
	@echo 'Invoking: MicroBlaze Print Size'
	mb-size board_test_app_Console.elf  |tee "board_test_app_Console.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

board_test_app_Console.elf.elfcheck: board_test_app_Console.elf
	@echo 'Invoking: Xilinx ELF Check'
	elfcheck board_test_app_Console.elf -hw ../../hw_platform/system.xml -pe microblaze_subsystem_microblaze_0  |tee "board_test_app_Console.elf.elfcheck"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) board_test_app_Console.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
