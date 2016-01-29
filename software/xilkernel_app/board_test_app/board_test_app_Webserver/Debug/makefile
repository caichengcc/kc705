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
board_test_app_Webserver.elf.size \

ELFCHECK += \
board_test_app_Webserver.elf.elfcheck \


# All Target
all: board_test_app_Webserver.elf secondary-outputs

# Tool invocations
board_test_app_Webserver.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MicroBlaze gcc linker'
	mb-gcc -L../../memfs -Wl,-T -Wl,../src/lscript.ld -L../../board_test_app_Webserver_platform/microblaze_subsystem_microblaze_0/lib -mlittle-endian -mcpu=v9.1 -mxl-soft-mul -Wl,--no-relax -Wl,--gc-sections -o "board_test_app_Webserver.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

board_test_app_Webserver.elf.size: board_test_app_Webserver.elf
	@echo 'Invoking: MicroBlaze Print Size'
	mb-size board_test_app_Webserver.elf  |tee "board_test_app_Webserver.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

board_test_app_Webserver.elf.elfcheck: board_test_app_Webserver.elf
	@echo 'Invoking: Xilinx ELF Check'
	elfcheck board_test_app_Webserver.elf -hw ../../hw_platform/system.xml -pe microblaze_subsystem_microblaze_0  |tee "board_test_app_Webserver.elf.elfcheck"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES)$(ELFSIZE)$(ELFCHECK)$(S_UPPER_DEPS) board_test_app_Webserver.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
